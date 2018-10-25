#include "Camera.h"
//#include "glm/gtc/matrix_transform.hpp"
#include <iostream> // remove

Camera::Camera()
{
	m_X = 0, m_Y = 0, m_Z = 0;
	m_Roll = 0, m_Pitch = 0, m_Yaw = 0;
	m_FocalLength = 0;
	m_RadialDistortion = 0;
	m_HorFOV = 0, m_VertFov = 0;
	m_ImageHeight = 0, m_ImageWidth = 0;
}

bool Camera::InitialiseValuesFromJSON(char* JSONFilepath)
{
	if (!JSONLoader::ReadJSONandFillMap(JSONFilepath))
	{
		return false;
	}

	std::multimap<std::string, float> ::iterator it;

	auto translationValues = JSONLoader::m_dataMap.equal_range("translation");
	int count = 0;

	for (auto i = translationValues.first; i != translationValues.second; ++i)
	{
		m_Position[count] = i->second;
		count++;
	}

	for (it = JSONLoader::m_dataMap.begin(); it != JSONLoader::m_dataMap.end(); it++)
	{	
		if (it->first == "roll" || it->first == "Roll") //  remove the capitalised part?
			m_Roll = it->second;

		else if (it->first == "pitch" || it->first == "Pitch")
			m_Pitch = it->second;

		else if (it->first == "yaw" || it->first == "Yaw")
			m_Yaw = it->second;

		else if (it->first == "focal length" || it->first == "Focal length")
			m_FocalLength = it->second;

		else if (it->first == "horizontal fov" || it->first == "Horizontal fov")
			m_HorFOV = it->second;

		else if (it->first == "radial distortion" || it->first == "Radial distortion")
			m_RadialDistortion = it->second;

		else if (it->first == "width" || it->first == "Width")
			m_ImageWidth = it->second;

		else if (it->first == "height" || it->first == "Height")
			m_ImageHeight = it->second;
	}

	m_VertFov = m_ImageHeight / m_ImageWidth * m_HorFOV;
	
	//PrintCameraContents();

	return true;
}

void Camera::CalculateGLMvalues()
{
	//// Pitch is rotation around x-axis
	//glm::mat4x4 pitchMatrix {1,0,0,0,
	//						 0,cos(m_Pitch),-sin(m_Pitch),0,
	//						 0,sin(m_Pitch),cos(m_Pitch),0,
	//						 0,0,0,1};

	//// Yaw is rotation around y-axis
	//glm::mat4x4 yawMatrix {cos(m_Yaw),0,sin(m_Yaw),0,
	//					   0,1,0,0,
	//					   -sin(m_Yaw),0,cos(m_Yaw),0,
	//					   0,0,0,1};

	//// Roll is rotation around z-axis
	//glm::mat4x4 rollMatrix {cos(m_Roll),-sin(m_Roll),0,0,
	//					   sin(m_Roll),cos(m_Roll),0,0,
	//	                   0,0,1,0,
	//	                   0,0,0,1};
	
	// Roll is rotation around z-axis
	glm::mat4x4 a{ cos(m_Roll),-sin(m_Roll),0,0,
		sin(m_Roll),cos(m_Roll),0,0,
		0,0,1,0,
		0,0,0,1 };
	
	//// Yaw is rotation around y-axis
	glm::mat4x4 b{ cos(m_Yaw),0,sin(m_Yaw),0,
			0,1,0,0,
			-sin(m_Yaw),0,cos(m_Yaw),0,
			0,0,0,1};

	// Roll is rotation around z-axis
	glm::mat4x4 c{ cos(m_Pitch),-sin(m_Pitch),0,0,
		sin(m_Pitch),cos(m_Pitch),0,0,
		0,0,1,0,
		0,0,0,1 };


	glm::mat4x4 cameraRotation = c * b*a;

	//glm::rotate(2.0f, glm::vec3(1, 0, 0));
	
	//glm::mat4x4 cameraRotation = rollMatrix * yawMatrix * pitchMatrix;
	//glm::mat4x4 cameraRotation = pitchMatrix * yawMatrix * rollMatrix;

	glm::mat4x4 other { cos(m_Roll)*cos(m_Yaw)*cos(m_Pitch) - sin(m_Roll)*sin(m_Pitch), -cos(m_Pitch)*sin(m_Roll) - cos(m_Roll)*cos(m_Yaw)*sin(m_Pitch), cos(m_Roll)*sin(m_Yaw), 0,
		cos(m_Yaw)*cos(m_Pitch)*sin(m_Roll) + cos(m_Roll)*sin(m_Pitch), cos(m_Roll)*cos(m_Pitch) - cos(m_Yaw)*sin(m_Roll)*sin(m_Pitch), sin(m_Roll)*sin(m_Yaw), 0,
		-cos(m_Pitch)*sin(m_Yaw), sin(m_Yaw)*sin(m_Pitch), cos(m_Yaw),0,
		0,0,0,1};

	PrintMat4x4(cameraRotation);
	PrintMat4x4(other);
}

void Camera::PrintCameraContents()
{
	std::cout << "Translation / Position: " << m_Position[0] << "," << m_Position[1] << "," << m_Position[2] << std::endl;
	std::cout << "Roll: " << m_Roll << std::endl;
	std::cout << "Pitch: " << m_Pitch << std::endl;
	std::cout << "Yaw: " << m_Yaw << std::endl;
	std::cout << "Focal Length: " << m_FocalLength << std::endl;
	std::cout << "Radial Distortion: " << m_RadialDistortion << std::endl;
	std::cout << "Horizontal FOV: " << m_HorFOV << std::endl;
	std::cout << "Vertical FOV: " << m_VertFov << std::endl;
	std::cout << "Image Height: " << m_ImageHeight << std::endl;
	std::cout << "Image Width: " << m_ImageWidth << std::endl;
}

void Camera::PrintMat4x4(glm::mat4x4 m)
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
