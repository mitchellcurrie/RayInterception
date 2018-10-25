// To DO

// Sort out multimap duplicates - instead of making new pair, check for existing key and add to it?

#include <iostream>
#include <time.h>
#include "ObjectData.h"
#include "ObjectLoader.h"
#include "Camera.h"
#include <string> // remove later

int main()
{
	// Load Object

	ObjectLoader* objLoader = new ObjectLoader();
	
	ObjectDataPtr objPtr(nullptr);

	// cin to get input for object filepath, then check for if objPtr is null / or do a cout in the loader saying loading... etc
	// if null, do a while loop to enter filepath again

	//clock_t tStart = clock();
	//objPtr = objLoader->Load("green.obj");
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);


	Camera c;
	if (!c.InitialiseValuesFromJSON(const_cast<char*>("camera.json")))
	{
		std::cout << "Couldn't initialse camera";
	}

	c.SetCameraDirection();






	int i;
	std::cin >> i;


	
	return 0;
}