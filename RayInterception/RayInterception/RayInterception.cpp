#include <iostream>
#include <map>
#include "RayInterception.h"
#include "ObjectData.h"

std::vector <VertexPosNrmTex> RayInterception::IndexOrderVertices;

RayInterception::RayInterception()
{
}

void RayInterception::UpdateObjectVertices(Camera _camera, ObjectDataPtr _objPtr)
{
	std::unordered_map<std::string, VertexCache>::iterator it;

	for (it = _objPtr->vertexCache.begin(); it != _objPtr->vertexCache.end(); it++)
	{
		// Convert position vec3 to vec4, adding 1 at the end for w
		glm::vec4 temp = glm::vec4(it->second.vertex.pos, 1);

		// Multiply position by MVP matrix
		temp = _camera.m_MVP * temp;

		// Assign the new position back to the vertex cache
		it->second.vertex.pos = temp;

		// Same calculations for the normal, multiplying by the normal matrix
		temp = glm::vec4(it->second.vertex.nrm, 1);
		temp = _camera.m_Normal * temp;
		it->second.vertex.nrm = temp;
	}
}

glm::vec3 RayInterception::CalculateRayFromScreenPoint(int _x, int _y, Camera _camera)
{
	// Need to move from screen space -> clip space -> view space -> world space
	// By reversing the camera transformations done to reach screen space
	
	////////////////////////////////
	// Screen space to clip space //
	////////////////////////////////

	// Convert coordinates to floats
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

void RayInterception::OrderVerticesBasedOnIndex(ObjectDataPtr _objPtr)
{
	IndexOrderVertices.resize(_objPtr->vertexCache.size());

	std::unordered_map<std::string, VertexCache>::iterator it;

	for (it = _objPtr->vertexCache.begin(); it != _objPtr->vertexCache.end(); it++)
	{
		IndexOrderVertices[it->second.index] = it->second.vertex;
	}

	//for (int j = 0; j < 10; j++)
	//{
	//	std::cout << j << "   " << IndexOrderVertices[j].pos.x << "," << IndexOrderVertices[j].pos.y << "," << IndexOrderVertices[j].pos.z << std::endl;
	//}
}

bool RayInterception::CalculateRayToObjectIntersection(glm::vec3 ray, ObjectDataPtr objPtr, Camera camera, glm::vec3 &intersect)
{	
	std::vector<glm::vec3> intersections;
	bool intersectionFound = false;
	
	for (int i = 0; i < objPtr->indices.size() - 2; i += 3)
	{
		if (GetRayTriangleIntersection(
			ray,
			IndexOrderVertices[objPtr->indices[i]].pos,
			IndexOrderVertices[objPtr->indices[i + 1]].pos,
			IndexOrderVertices[objPtr->indices[i + 2]].pos,
			camera,
			intersect))
		{
			intersectionFound = true;
			intersections.push_back(intersect);
		}
	}

	if (!intersectionFound)
	{
		return false;
	}

	float minDistance = -1.0f;
	int index = 0;

	//std::cout << "Size of intersections vector: " << intersections.size() << std::endl;

	for (int j = 0; j < intersections.size(); j++)
	{
		float distance = glm::length(intersections[j] - glm::vec3(camera.m_Position));

		if (distance < minDistance)
		{
			index = j;
			distance = minDistance;
		}
	}

	intersect = intersections[index];

	return true;
}

bool RayInterception::GetRayTriangleIntersection(glm::vec3 ray, glm::vec3 triIndex_1, glm::vec3 triIndex_2, glm::vec3 triIndex_3, Camera camera, glm::vec3 & intersect)
{
	// First check if the ray intersects with the triangle plane
	
	// Calculate triangle normal
	glm::vec3 triangleNormal = glm::cross(triIndex_2 - triIndex_1, triIndex_3 - triIndex_1);

	// Calculate dot product of triangle normal and ray 
	float dot1 = glm::dot(triangleNormal, ray);

	// If dot 1 == 0, tri nrm and ray are perpendicular, therefore the triangle plane and the ray are parallel
	if (dot1 == 0)
	{
		// Check if ray is contained within the triangle plane by calculating the dot product of the trianlge normal and camera to triangle point vector
		float dot2 = glm::dot(triangleNormal, glm::vec3(camera.m_Position) - triIndex_1);

		if (dot2 != 0)
		{
			// Line is disjointed from plane and no intersection
			return false;
		}

		// Ray is contained within the triangle plane, therefore intersects at every point
		// Intersection is inifinity and therefore not intersecting???
	}

	// Intersection exists

	// Ray equation = Point on Ray + t * Ray direction

	// t = -(triNorm.CameraPos + d) / (N.D)

	// t = d - (n . p) / n . d   
	/*glm::vec3 t = (ray - (glm::dot(triangleNormal, glm::vec3(camera.m_Position)))) /
			      (glm::dot(triangleNormal, ray));*/

	//float t = (glm::dot((ray - triangleNormal), glm::vec3(camera.m_Position))) / 
	//	      (glm::dot(triangleNormal, ray));

	float t = (glm::dot(triangleNormal, triIndex_1) - glm::dot(glm::vec3(camera.m_Position), triangleNormal)) /
				glm::dot(ray, triangleNormal);
			     



	// Putting t back into ray equation
	intersect = glm::vec3(camera.m_Position) + t * ray;


	// Check if intersection is within the triangle

	// Need to check against all edges of the triangle
	// Inside triangle if all are true:

	// [(B - A) x (Q - A)] . n >= 0
	// [(C - B) x (Q - B)] . n >= 0
	// [(A - C) x (Q - C)] . n >= 0

	if (glm::dot(glm::cross(triIndex_2 - triIndex_1, intersect - triIndex_1), triangleNormal) >= 0 &&
		glm::dot(glm::cross(triIndex_3 - triIndex_2, intersect - triIndex_2), triangleNormal) >= 0 &&
		glm::dot(glm::cross(triIndex_1 - triIndex_3, intersect - triIndex_3), triangleNormal) >= 0)
	{
		return true;
	}

	return false;
}
