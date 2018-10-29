#include <iostream>
#include "ConsolePrinter.h"

void ConsolePrinter::PrintVec3(glm::vec3 v)
{
	std::cout << v.x << ", " << v.y << ", " << v.z << "\n\n";
}

void ConsolePrinter::PrintUserInputAndInfo(int Ps1_x, int Ps1_y, int Ps2_x, int Ps2_y)
{
	std::cout << "Ps1:  " << Ps1_x << " , " << Ps1_y << "\nPs2:  " << Ps2_x << " , " << Ps2_y << "\n\n";
	std::cout << "Ray1 and Ray2 are calculated from screen positions Ps1 and Ps2 (in pixels).  \n\n";
}

void ConsolePrinter::PrintRayInterceptionResults(bool Ray1Intercepted, bool Ray2Intercepted, glm::vec3 Ray1InterceptionPoint, glm::vec3 Ray2InterceptionPoint)
{
	if (Ray1Intercepted)
	{
		std::cout << "Ray1 Interception with mesh at: ";
		PrintVec3(Ray1InterceptionPoint);
	}

	if (Ray2Intercepted)
	{
		std::cout << "Ray2 Interception with mesh at: ";
		PrintVec3(Ray2InterceptionPoint);
	}

	if (Ray1Intercepted && Ray2Intercepted)
	{
		float distance = glm::length(Ray2InterceptionPoint - Ray1InterceptionPoint);
		std::cout << "\nDistance between interception points: " << distance << "\n\n\n";
	}
	else if (!Ray1Intercepted && !Ray2Intercepted)
	{
		std::cout << "Neither Ray1 or Ray2 intercepted with the mesh. \n\n\n";
	}

	else // Only 1 ray intercepted - can't compute distance
	{
		std::cout << "\nDistance calculation between interception points not possible as only one ray intercepted with the mesh. \n\n\n";
	}
}

