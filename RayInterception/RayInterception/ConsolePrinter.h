#pragma once
#include <glm\glm.hpp>

class ConsolePrinter
{
public:
	static void PrintVec3(glm::vec3);
	static void PrintUserInputAndInfo(int Ps1_x, int Ps1_y, int Ps2_x, int Ps2_y);
	static void PrintRayInterceptionResults(bool Ray1Intercepted, bool Ray2Intercepted, glm::vec3 Ray1InterceptionPoint, glm::vec3 Ray2InterceptionPoint);
};

