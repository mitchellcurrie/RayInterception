#include <iostream>
#include <time.h>
#include "ObjectData.h"
#include "ObjectLoader.h"
#include "JSONLoader.h"
#include <string> // remove later
int main()
{
	// Load Object

	ObjectLoader* objLoader = new ObjectLoader();
	
	ObjectDataPtr objPtr(nullptr);

	//clock_t tStart = clock();
	//objPtr = objLoader->Load("green.obj");
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);


	// Load JSON - to be done in JSON loader class

	JSONLoader jsl;
	jsl.ReadFromJSONFile("camera.json");





	int i;
	std::cin >> i;


	
	return 0;
}