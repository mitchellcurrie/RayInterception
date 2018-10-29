// To DO

// Go through map of vertices and multiply them by the mvp matrix
// Get click screen points - convert to world space and determine where ray hit
// Check that world position using ray to triangle interection on each face of the object
// When looping through all, get dot product of surface normal etc to see if face is facing camera and ignore?

// SWAP ENDLs for \n for ones being kept in

// Links
// https://webglfundamentals.org/webgl/lessons/webgl-3d-camera.html
// https://www.3dgep.com/understanding-the-view-matrix/
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/#the-model-view-and-projection-matrices
// https://wiki.panotools.org/Field_of_View
// https://www.opengl.org/discussion_boards/showthread.php/156169-How-to-set-focal-length-in-OpenGL-_

// Mouse coordinates
// https://gamedev.stackexchange.com/questions/9693/whats-a-good-way-to-check-that-a-player-has-clicked-on-an-object-in-a-3d-game
// http://antongerdelan.net/opengl/raycasting.html
// http://schabby.de/picking-opengl-ray-tracing/
// https://stackoverflow.com/questions/40276068/opengl-raycasting-with-any-object

// Ray triangle intersection questions with answers to test
// https://math.stackexchange.com/questions/1979876/ray-casting-algorithm-in-ray-triangle-intersection
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
// https://www.gamedev.net/forums/topic/321357-ray-plane-intersection/


#include <iostream>
#include <time.h>
#include "ObjectData.h"
#include "ObjectLoader.h"
#include "Camera.h"
#include "RayInterception.h"

void PrintVertexCacheElements(ObjectDataPtr _objPtr, int _numOfElements); // remove
void PrintAllVertexCacheElements(ObjectDataPtr _objPtr);
void PrintIndicesVector(ObjectDataPtr _objPtr);

void GetScreenCoordinatesInput(short &x1, short &y1, short &x2, short &y2);
void PrintVec3(glm::vec3);
void PrintRayInterceptionResults(bool Ray1Intercepted, bool Ray2Intercepted, glm::vec3 Ray1InterceptionPoint, glm::vec3 Ray2InterceptionPoint);

int main()
{
	std::cout << "RAY INTERCEPTION CALCULATOR\n" << std::endl;

	short x1, y1, x2, y2;

	GetScreenCoordinatesInput(x1, y1, x2, y2);

	///////////////////
	// Set up Camera //
	///////////////////

	Camera camera;
	if (!camera.InitialiseValuesFromJSON(const_cast<char*>("camera.json")))
	{
		std::cout << "ERROR: Could not initialise camera.\n";
		system("pause");
		return 0;
	}

	camera.SetMatrices();

	/////////////////
	// Load object //
	/////////////////

	ObjectDataPtr objPtr(nullptr);

	clock_t tStart = clock();
	std::cout << "Loading Mesh Object... Please wait\n";
	objPtr = ObjectLoader::Load("green.obj");

	if (!objPtr)
	{
		std::cout << "ERROR: Mesh object file failed to load.\n";
		system("pause");
		return 0;
	}

	printf("Time taken to load object: %.2fs\n\n\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	///////////////////////////////////////////////////////
	// Update object vertices based on camera and reorder//
	//////////////////////////////////////////////////////

	RayInterception::UpdateObjectVertices(camera, objPtr);
	RayInterception::OrderVerticesBasedOnIndex(objPtr);

	/////////////////////////////////
	// Calculate Ray Intersections //
	/////////////////////////////////

	glm::vec3 Ray1 = RayInterception::CalculateRayFromScreenPoint(x1, y1, camera);
	glm::vec3 Ray2 = RayInterception::CalculateRayFromScreenPoint(x2, y2, camera);

	glm::vec3 Ray1InterceptionPoint, Ray2InterceptionPoint;

	bool Ray1Intercepted = RayInterception::CalculateRayToObjectIntersection(Ray1, objPtr, camera, Ray1InterceptionPoint);
	bool Ray2Intercepted = RayInterception::CalculateRayToObjectIntersection(Ray2, objPtr, camera, Ray2InterceptionPoint);

	PrintRayInterceptionResults(Ray1Intercepted, Ray2Intercepted, Ray1InterceptionPoint, Ray2InterceptionPoint);

	system("pause");
	return 0;
}


void PrintVec3(glm::vec3 v)
{
	std::cout << v.x << ", " << v.y << ", " << v.z << "\n\n";
}

void PrintRayInterceptionResults(bool Ray1Intercepted, bool Ray2Intercepted, glm::vec3 Ray1InterceptionPoint, glm::vec3 Ray2InterceptionPoint)
{
	if (Ray1Intercepted)
	{
		std::cout << "Ray 1 Interception with Object at: ";
		PrintVec3(Ray1InterceptionPoint);
	}

	if (Ray2Intercepted)
	{
		std::cout << "Ray 2 Interception with Object at: ";
		PrintVec3(Ray2InterceptionPoint);
	}

	if (Ray1Intercepted && Ray2Intercepted)
	{
		float distance = glm::length(Ray2InterceptionPoint - Ray1InterceptionPoint);
		std::cout << "\nDistance between interception points: " << distance << "\n\n\n";
	}
	else if (!Ray1Intercepted && !Ray2Intercepted)
	{
		std::cout << "Neither Ray 1 or Ray 2 intercepted with the object. \n\n\n";
	}

	else // Only 1 ray intercepted - can't compute distance
	{
		std::cout << "\nDistance calculatation between interception points not possible as only one ray intersected the object. \n\n\n";
	}
}

void GetScreenCoordinatesInput(short & x1, short & y1, short & x2, short & y2)
{
	std::cout << "Please enter the first screen space coordinate (in pixels) for Ray 1:" << std::endl << "x: ";
	std::cin >> x1;
	std::cout << "y: ";
	std::cin >> y1;

	std::cout << std::endl;

	std::cout << "Please enter the second screen space coordinate (in pixels) for Ray 2:" << std::endl << "x: ";
	std::cin >> x2;
	std::cout << "y: ";
	std::cin >> y2;

	std::cout << std::endl;
}









void PrintVertexCacheElements(ObjectDataPtr _objPtr, int _numOfElements) // to remove
{
	std::unordered_map<std::string, VertexCache>::iterator it;
	it = _objPtr->vertexCache.begin();

	for (int i = 0; i < _numOfElements; i++)
	{
		std::cout << "String: " << it->first << "   Index: " << it->second.index << "    Pos: "
			<< it->second.vertex.pos.x << "," << it->second.vertex.pos.y << "," << it->second.vertex.pos.z << "   Nrm: "
			<< it->second.vertex.nrm.x << "," << it->second.vertex.nrm.y << "," << it->second.vertex.nrm.z;
		it++;


		std::cout << "   i: " << _objPtr->indices[i] << std::endl;

	}

	std::cout << std::endl;
}

void PrintAllVertexCacheElements(ObjectDataPtr _objPtr) // to remove
{
	std::unordered_map<std::string, VertexCache>::iterator it;

	for (it = _objPtr->vertexCache.begin(); it != _objPtr->vertexCache.end(); it++)
	{
		if (it->second.index > 100 && it->second.index < 131)
		{
			std::cout << "String: " << it->first << "   Index: " << it->second.index << "    Pos: "
				<< it->second.vertex.pos.x << "," << it->second.vertex.pos.y << "," << it->second.vertex.pos.z << "   Nrm: "
				<< it->second.vertex.nrm.x << "," << it->second.vertex.nrm.y << "," << it->second.vertex.nrm.z;

			std::cout << "   i: " << _objPtr->indices[it->second.index] << std::endl;
		}
	}

	std::cout << std::endl;
}

void PrintIndicesVector(ObjectDataPtr _objPtr)
{
	//_objPtr->indices.size()

	for (int i = 0; i < _objPtr->indices.size(); i++)
	{
		if (_objPtr->indices[i] < 20)
		{
			std::cout << "i: " << i << "   Index: " << _objPtr->indices[i] << std::endl;
		}
	}
}

// Ray triangle intersect test

//glm::vec3 triA{ -10,-2.3,0 };
//glm::vec3 triB{ 4.4,20.3,9.5 };
//glm::vec3 triC{ 9.8,-10,0 };
//glm::vec3 rayOrg{ 0,0,0 };
//glm::vec3 rayDir{ 0.68, -1.14, 1.82 };
//
//camera.m_Position = glm::vec4(rayOrg, 1);
//
//glm::vec3 intersect{ 0 };
//
//
//if (RayInterception::GetRayTriangleIntersection(rayDir, triA, triB, triC, camera, intersect))
//{
//	std::cout << "Intersection: " << intersect.x << "," << intersect.y << "," << intersect.z << std::endl;
//	std::cout << "Distance: " << glm::length(intersect - glm::vec3(camera.m_Position)) << std::endl;


/*float x = 440;
float y = 540;*/