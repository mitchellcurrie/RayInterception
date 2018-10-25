#include "Camera.h"
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

bool Camera::Initialise(char* JSONFilepath)
{
	if (!JSONLoader::ReadJSONandFillMap(JSONFilepath))
	{
		return false;
	}

	std::multimap<std::string, float> ::iterator it;

	auto translationValues = JSONLoader::m_dataMap.equal_range("translation");
	float valuesXYZ[3] = { 0,0,0 };
	int count = 0;

	for (auto i = translationValues.first; i != translationValues.second; ++i)
	{
		valuesXYZ[count] = i->second;
		count++;
	}

	m_X = valuesXYZ[0];
	m_Y = valuesXYZ[1];
	m_Z = valuesXYZ[2];

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
	
	PrintCameraContents();

	return true;
}

void Camera::PrintCameraContents()
{
	std::cout << "Translation: " << m_X << "," << m_Y << "," << m_Z << std::endl;
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
