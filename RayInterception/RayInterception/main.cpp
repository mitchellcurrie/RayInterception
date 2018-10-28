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

int main()
{
	std::cout << "Ray Interception Calculator\n" << std::endl;

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
	std::cout << "Loading Mesh Object... Please wait" << std::endl;  //change to if actually loading etc
	objPtr = ObjectLoader::Load("green.obj");

	if (!objPtr)
	{
		std::cout << "ERROR: Mesh object file failed to load.\n";
		system("pause");
		return 0;
	}

	printf("Time taken to load object: %.2fs\n\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

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

	glm::vec3 Intersection;
	glm::vec3 Intersection2;

	if (RayInterception::CalculateRayToObjectIntersection(Ray1, objPtr, camera, Intersection))
	{
		std::cout << "Intersection!  At: " << Intersection.x << "," << Intersection.y << "," << Intersection.z << std::endl << std::endl;
	}
	else
	{
		std::cout << "No Intersection" << std::endl << std::endl;
	}


	system("pause");
	return 0;
}


void GetScreenCoordinatesInput(short & x1, short & y1, short & x2, short & y2)
{
	std::cout << "Please enter the first screen space coordinate:" << std::endl << "x: ";
	std::cin >> x1;
	std::cout << "y: ";
	std::cin >> y1;

	std::cout << std::endl;

	std::cout << "Please enter the second screen space coordinate:" << std::endl << "x: ";
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