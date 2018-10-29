#include "RayInterception.h"
#include "ObjectData.h"

#include <map>

std::vector <VertexPosNrmTex> RayInterception::IndexOrderVertices;

RayInterception::RayInterception()
{
}

// Updates the mesh object vertices based on the MVP and normal matrices
void RayInterception::UpdateObjectVertices(Camera _camera, ObjectDataPtr _objPtr)
{
	std::unordered_map<std::string, VertexCache>::iterator it;

	// Iterate through vertex cache
	for (it = _objPtr->vertexCache.begin(); it != _objPtr->vertexCache.end(); it++)
	{
		// Convert position vec3 to vec4, adding 1 at the end for w, to represent a position vector (need vec4 to multiply with MVP matrix)
		glm::vec4 temp = glm::vec4(it->second.vertex.pos, 1);

		// Multiply position by MVP matrix
		temp = _camera.m_MVP * temp;

		// Assign the new position back to the vertex cache
		it->second.vertex.pos = temp;

		// Same calculations for the normal, but instead multiplying by the normal matrix
		temp = glm::vec4(it->second.vertex.nrm, 1);
		temp = _camera.m_Normal * temp;
		it->second.vertex.nrm = temp;
	}
}
// Returns a vec3 ray based on the screen coordinates and camera
glm::vec3 RayInterception::CalculateRayFromScreenPoint(int _x, int _y, Camera _camera)
{
	// Need to move from screen space -> clip space -> view space -> world space,
	// by reversing the camera transformations done to reach screen space
	
	////////////////////////////////
	// Screen space to clip space //
	////////////////////////////////

	// Convert screen coordinates to floats
	float x = static_cast<float>(_x);
	float y = static_cast<float>(_y);

	// Translate x and y to move origin to centre of view port
	x -= _camera.m_ImageWidth / 2;
	y -= _camera.m_ImageHeight / 2;

	// Scale x and y to the range of -1 to 1
	x /= (_camera.m_ImageWidth / 2);
	y /= (_camera.m_ImageHeight / 2);

	// Create vec4 with x, y, z = 1 and w = 1
	// z = 1 because ray goes into screen which is positive z in a left handed camera.
	// w = 1 to form 4d vector to be able to multiply with 4x4 matrices
	glm::vec4 clipRay = glm::vec4(x, y, 1.0f, 1.0f);

	//////////////////////////////
	// Clip Space to View Space //
	//////////////////////////////

	// Reverse projection matrix transformation
	glm::vec4 viewRay = glm::inverse(_camera.m_Projection) * clipRay;

	// We only need the reversed x and y
	// Set Z to 1 as the ray moves into the screen and w to 0 to represent a ray rather than a point

	viewRay = glm::vec4(viewRay.x, viewRay.y, -1.0f, 0.0f);

	///////////////////////////////
	// View Space to World Space //
	///////////////////////////////

	// Inverse view matrix transformation
	glm::vec4 worldRay = glm::inverse(_camera.m_View) * viewRay;

	// Normalize
	worldRay = glm::normalize(worldRay);

	// Return xyz of worldRay
	return glm::vec3(worldRay);	
}

// Populates vector "IndexOrderVertices" with all mesh vertices, with the vertex index as the vector index
// Resulting in a vector storing all mesh vertices in order of index - for use in "CalculateRayToObjectInterception" function
void RayInterception::OrderVerticesBasedOnIndex(ObjectDataPtr _objPtr)
{
	IndexOrderVertices.resize(_objPtr->vertexCache.size());

	std::unordered_map<std::string, VertexCache>::iterator it;

	for (it = _objPtr->vertexCache.begin(); it != _objPtr->vertexCache.end(); it++)
	{
		IndexOrderVertices[it->second.index] = it->second.vertex;
	}
}

bool RayInterception::CalculateRayToObjectInterception(glm::vec3 ray, ObjectDataPtr objPtr, Camera camera, glm::vec3 &intercept)
{	
	// Create vector to store all interception points
	std::vector<glm::vec3> interceptions;

	bool interceptionFound = false;
	
	//Iterate through indices, where each 3 successive values represent a face (triangle) of the mesh
	for (int i = 0; i < objPtr->indices.size() - 2; i += 3)
	{
		// Check if ray intercepts triangle made up of the 3 indices
		if (GetRayTriangleInterception(
			ray,													
			IndexOrderVertices[objPtr->indices[i]].pos,				// Triangle index 1
			IndexOrderVertices[objPtr->indices[i + 1]].pos,         // Triangle index 2
			IndexOrderVertices[objPtr->indices[i + 2]].pos,			// Triangle index 3
			camera,													
			intercept))												// reference to interception point
		{
			interceptionFound = true;

			// Interception occurred, store resulting intercept vec3 in vector of interceptions
			interceptions.push_back(intercept);
		}
	}

	// No interceptions found with ray and all mesh faces
	if (!interceptionFound)
	{
		return false;
	}

	// Check which interception point is closest to the camera world position
	float minDistance = -1.0f;
	int index = 0;

	for (int j = 0; j < interceptions.size(); j++)
	{
		// Calculate distance between interception point and camera world position
		float distance = glm::length(interceptions[j] - glm::vec3(camera.m_Position));

		if (distance < minDistance)
		{
			index = j;
			distance = minDistance;
		}
	}

	// Intercept equals item in vector with lowest distance value
	intercept = interceptions[index];

	return true;
}

bool RayInterception::GetRayTriangleInterception(glm::vec3 ray, glm::vec3 triIndex_1, glm::vec3 triIndex_2, glm::vec3 triIndex_3, Camera camera, glm::vec3 & intercept)
{	
	///////////////////////////////////////////////////////////////
	// First check if the ray intercepts with the triangle plane //
	///////////////////////////////////////////////////////////////

	// Calculate triangle normal
	glm::vec3 triangleNormal = glm::cross(triIndex_2 - triIndex_1, triIndex_3 - triIndex_1);

	// Calculate dot product of triangle normal and ray 
	float dot1 = glm::dot(triangleNormal, ray);

	// If dot1 == 0, tri nrm and ray are perpendicular, therefore the triangle plane and the ray are parallel
	if (dot1 == 0)
	{
		// Check if ray is contained within the triangle plane by calculating the dot product of the trianlge normal and camera to triangle point vector
		float dot2 = glm::dot(triangleNormal, glm::vec3(camera.m_Position) - triIndex_1);

		if (dot2 != 0)
		{
			// Line is disjointed from plane and no interception
			return false;
		}

		// Ray is contained within the triangle plane, therefore intercepts at every point
		// Interception is inifinity and can't calculate interception point

		return false;
	}

	////////////////////////////////////
	// Interception with plane exists //
	////////////////////////////////////

	// Ray equation = Point on Ray + t * Ray direction

	// Need to calculate t:

	float t = (glm::dot(triangleNormal, triIndex_1) - glm::dot(glm::vec3(camera.m_Position), triangleNormal)) /
				glm::dot(ray, triangleNormal);
			     
	// Putting t back into ray equation
	intercept = glm::vec3(camera.m_Position) + t * ray;

	//////////////////////////////////////////////////
	// Check if interception is within the triangle //
	//////////////////////////////////////////////////

	// Need to check interception pooint against all edges of the triangle to see if point is inside or outside
	// Point is inside triangle if all of the following are true:

	if (glm::dot(glm::cross(triIndex_2 - triIndex_1, intercept - triIndex_1), triangleNormal) >= 0 &&
		glm::dot(glm::cross(triIndex_3 - triIndex_2, intercept - triIndex_2), triangleNormal) >= 0 &&
		glm::dot(glm::cross(triIndex_1 - triIndex_3, intercept - triIndex_3), triangleNormal) >= 0)
	{
		return true;
	}

	return false;
}
