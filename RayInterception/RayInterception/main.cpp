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



#include <iostream>
#include <time.h>
#include "ObjectData.h"
#include "ObjectLoader.h"
#include "Camera.h"
#include <string> // remove later

int main()
{
	// Load Object
	ObjectDataPtr objPtr(nullptr);

	// cin to get input for object filepath, then check for if objPtr is null / or do a cout in the loader saying loading... etc
	// if null, do a while loop to enter filepath again

	clock_t tStart = clock();
	std::cout << "Loading..." << std::endl;  //change to if actually loading etc
	//objPtr = ObjectLoader::Load("green.obj");
	printf("Time taken to load object: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);


	Camera camera;
	if (!camera.InitialiseValuesFromJSON(const_cast<char*>("camera.json")))
	{
		std::cout << "Couldn't initialse camera";
	}

	camera.SetMatrices();

	int s;
	std::cin >> s;

	//std::cout << std::endl << "Size of indices vector: " << objPtr->indices.size() << std::endl;

	//std::cout << "Size of vertex cache map: " << objPtr->vertexCache.size() << std::endl;

	std::unordered_map<std::string, VertexCache>::iterator it;

	it = objPtr->vertexCache.begin();
	
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Index: " << it->second.index << "    Vertex: "
				  << it->second.vertex.pos[0] << "," << it->second.vertex.pos[1] << "," << it->second.vertex.pos[2] << std::endl;
		it++;
	}

	std::cout << std::endl;

	tStart = clock();
	std::cout << "Looping..." << std::endl;

	for (it = objPtr->vertexCache.begin(); it != objPtr->vertexCache.end(); it++)
	{
		glm::vec4 temp{ it->second.vertex.pos[0], it->second.vertex.pos[1], it->second.vertex.pos[2], 1 };
		temp = camera.m_MVP * temp;
		it->second.vertex.pos[0] = temp[0];
		it->second.vertex.pos[1] = temp[1];
		it->second.vertex.pos[2] = temp[2];
	}

	printf("Time taken to loop: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	it = objPtr->vertexCache.begin();

	for (int i = 0; i < 10; i++)
	{
		std::cout << "Index: " << it->second.index << "    Vertex: "
			<< it->second.vertex.pos[0] << "," << it->second.vertex.pos[1] << "," << it->second.vertex.pos[2] << std::endl;
		it++;
	}

	int w;
	std::cin >> w;


	
	return 0;
}