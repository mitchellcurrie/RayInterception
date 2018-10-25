#include <iostream>
#include "Utils.h"

void Utils::PrintMat4(glm::mat4 m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << m[i][j] << ", ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}
void Utils::PrintVec4(glm::vec4 v)
{
	std::cout << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << std::endl;
}

