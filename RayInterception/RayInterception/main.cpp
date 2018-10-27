// To DO

// Go through map of vertices and multiply them by the mvp matrix
// Get click screen points - convert to world space and determine where ray hit
// Check that world position using ray to triangle interection on each face of the object
// When looping through all, get dot product of surface normal etc to see if face is facing camera and ignore?

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



#include <iostream>
#include <time.h>
#include "ObjectData.h"
#include "ObjectLoader.h"
#include "Camera.h"
#include "RayInterception.h"

void PrintVertexCacheElements(ObjectDataPtr _objPtr, int _numOfElements); // remove
void PrintAllVertexCacheElements(ObjectDataPtr _objPtr);
void PrintIndicesVector(ObjectDataPtr _objPtr);

int main()
{
	/////////////////
	// Load object //
	/////////////////

	ObjectDataPtr objPtr(nullptr);

	// cin to get input for object filepath, then check for if objPtr is null / or do a cout in the loader saying loading... etc
	// if null, do a while loop to enter filepath again

	clock_t tStart = clock();
	std::cout << "Loading..." << std::endl;  //change to if actually loading etc
	objPtr = ObjectLoader::Load("green.obj");
	printf("Time taken to load object: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	///////////////////
	// Set up Camera //
	///////////////////

	Camera camera;
	if (!camera.InitialiseValuesFromJSON(const_cast<char*>("camera.json")))
	{
		std::cout << "Couldn't initialse camera";
	}
	else
	{
		camera.SetMatrices();
	}

	////////////////////////////////////////////
	// Update object vertices based on camera //
	///////////////////////////////////////////

	if (objPtr)
	{
		RayInterception::UpdateObjectVertices(camera, objPtr);
		//PrintAllVertexCacheElements(objPtr);
		PrintIndicesVector(objPtr);
	}

	float x = 1530;
	float y = 900;

	std::cout << x << "," << y << std::endl;

	glm::vec3 R1 = RayInterception::CalculateRayFromScreenPoint(x, y, camera);

	std::cout << R1.x << "," << R1.y << "," << R1.z << std::endl;

	int w;
	std::cin >> w;

	return 0;
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
	it = _objPtr->vertexCache.begin();

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

	for (int i = 0; i < 100; i++)
	{
		std::cout << "i: " << i << "   Index: " << _objPtr->indices[i] << std::endl;
	}
}