// SWAP ENDLs for \n for ones being kept in
// Inlclude order according to object loader .cpp
#include "ObjectData.h"
#include "ObjectLoader.h"
#include "Camera.h"
#include "RayInterception.h"
#include "ConsolePrinter.h"

#include <iostream>
#include <time.h>

int main(int argc, char *argv[])
{
	std::cout << "\nRAY INTERCEPTION CALCULATOR\n\n";

	////////////////////////////////
	// Check and store user input //
	////////////////////////////////

	// Check for the correct number of arguments passed to application
	if (argc != 5) // if user enters 4 arguments, argc equals 5
	{
		std::cout << "Invalid number of arguments.\n\nPlease enter 4 arguments in this order:\nPs1_x , Ps1_y , Ps2_x , Ps2_y.\n\n";
		system("pause");
		return 0;
	}
	
	// Convert and store command line arguments as integers
	int Ps1_x, Ps1_y, Ps2_x, Ps2_y;

	Ps1_x = atoi(argv[1]);
	Ps1_y = atoi(argv[2]);
	Ps2_x = atoi(argv[3]);
	Ps2_y = atoi(argv[4]);

	ConsolePrinter::PrintUserInputAndInfo(Ps1_x, Ps1_y, Ps2_x, Ps2_y);

	///////////////////
	// Set up Camera //
	///////////////////

	Camera camera;
	if (!camera.InitialiseValuesFromJSON(const_cast<char*>("Assets/camera.json")))
	{
		std::cout << "ERROR: Could not initialise camera.\n\n";
		system("pause");
		return 0;
	}

	// Set all camera matrices - M, V, P etc
	camera.SetMatrices();

	//////////////////////
	// Load mesh object //
	//////////////////////

	ObjectDataPtr MeshPtr(nullptr);

	clock_t tStart = clock();
	std::cout << "Loading Mesh... Please wait (can take up to 45 seconds)\n";
	MeshPtr = ObjectLoader::Load("Assets/green.obj");

	if (!MeshPtr)
	{
		std::cout << "\nERROR: Mesh object file failed to load.\n\n";
		system("pause");
		return 0;
	}

	printf("Time taken to load mesh object: %.2fs\n\n\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	///////////////////////////////////////////////////////
	// Update object vertices based on camera and reorder//
	//////////////////////////////////////////////////////

	RayInterception::UpdateObjectVertices(camera, MeshPtr);
	RayInterception::OrderVerticesBasedOnIndex(MeshPtr);

	/////////////////////////////////
	// Calculate Ray Interceptions //
	/////////////////////////////////

	glm::vec3 Ray1 = RayInterception::CalculateRayFromScreenPoint(Ps1_x, Ps1_y, camera);
	glm::vec3 Ray2 = RayInterception::CalculateRayFromScreenPoint(Ps2_x, Ps2_y, camera);

	glm::vec3 Ray1InterceptionPoint, Ray2InterceptionPoint;

	bool Ray1Intercepted = RayInterception::CalculateRayToObjectInterception(Ray1, MeshPtr, camera, Ray1InterceptionPoint);
	bool Ray2Intercepted = RayInterception::CalculateRayToObjectInterception(Ray2, MeshPtr, camera, Ray2InterceptionPoint);

	ConsolePrinter::PrintRayInterceptionResults(Ray1Intercepted, Ray2Intercepted, Ray1InterceptionPoint, Ray2InterceptionPoint);

	system("pause");
	return 0;
}