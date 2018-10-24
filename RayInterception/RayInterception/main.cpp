#include <iostream>
#include <time.h>
#include "ObjectData.h"
#include "ObjectLoader.h"

int main()
{
	// Load Object

	ObjectLoader* objLoader = new ObjectLoader();
	
	ObjectDataPtr objPtr(nullptr);

	//clock_t tStart = clock();

	objPtr = objLoader->Load("green.obj");

	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);



	int i;
	std::cin >> i;


	
	return 0;
}