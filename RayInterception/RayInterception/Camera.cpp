#include "Camera.h"
#include "Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream> // remove

Camera::Camera()
{
	m_Position = glm::vec4(0);
	m_Position.w = 1.0f;
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

	//PrintCameraContents();

	return true;
}

void Camera::SetRotation()
{
	//// ORIGINAL //
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
	

	//// ZYZ //
	//// Rotate around z-axis by pitch
	//glm::mat4x4 a{ cos(m_Pitch),-sin(m_Pitch),0,0,
	//	sin(m_Pitch),cos(m_Pitch),0,0,
	//	0,0,1,0,
	//	0,0,0,1 };
	//
	//// Rotate around y-axis by yaw
	//glm::mat4x4 b{ cos(m_Yaw),0,sin(m_Yaw),0,
	//		0,1,0,0,
	//		-sin(m_Yaw),0,cos(m_Yaw),0,
	//		0,0,0,1};

	//// Rotate around z-axis by roll
	//glm::mat4x4 c{ cos(m_Roll),-sin(m_Roll),0,0,
	//	sin(m_Roll),cos(m_Roll),0,0,
	//	0,0,1,0,
	//	0,0,0,1 };


	//// Euler ZXZ Rotation Format //
	// Rotate around z-axis by pitch
	glm::mat4 zPitch {cos(m_Pitch),-sin(m_Pitch),0,0,
						sin(m_Pitch),cos(m_Pitch),0,0,
						0,0,1,0,
						0,0,0,1 };

	// Rotate around x-axis by yaw
	glm::mat4 xYaw {1,0,0,0,
					  0,cos(m_Yaw),-sin(m_Yaw),0,
					  0,sin(m_Yaw),cos(m_Yaw),0,
					  0,0,0,1 };

	// Rotate around z-axis by roll
	glm::mat4 zRoll {cos(m_Roll),-sin(m_Roll),0,0,
				       sin(m_Roll),cos(m_Roll),0,0,
					   0,0,1,0,
					   0,0,0,1 };

	m_Rotation = zPitch * xYaw * zRoll;

	



	
	//glm::mat4x4 cameraRotation = rollMatrix * yawMatrix * pitchMatrix;
	//glm::mat4x4 cameraRotation = pitchMatrix * yawMatrix * rollMatrix;

	/*glm::mat4x4 ZYZ { cos(m_Roll)*cos(m_Yaw)*cos(m_Pitch) - sin(m_Roll)*sin(m_Pitch), -cos(m_Pitch)*sin(m_Roll) - cos(m_Roll)*cos(m_Yaw)*sin(m_Pitch), cos(m_Roll)*sin(m_Yaw), 0,
		cos(m_Yaw)*cos(m_Pitch)*sin(m_Roll) + cos(m_Roll)*sin(m_Pitch), cos(m_Roll)*cos(m_Pitch) - cos(m_Yaw)*sin(m_Roll)*sin(m_Pitch), sin(m_Roll)*sin(m_Yaw), 0,
		-cos(m_Pitch)*sin(m_Yaw), sin(m_Yaw)*sin(m_Pitch), cos(m_Yaw),0,
		0,0,0,1};*/
}

void Camera::SetTranslation()
{
	m_Translation = glm::mat4(1, 0, 0, m_Position.x,
							  0, 1, 0, m_Position.y,
							  0, 0, 1, m_Position.z,
							  0, 0, 0, m_Position.w);
}

void Camera::SetMatrices()
{
	SetRotation();
	SetTranslation();

	// Set camera matrix to identity
	m_Camera = glm::mat4(1.0);

	// Rotate by rotation matrix
	m_Camera *= m_Rotation;

	// Translate by translation matrix
	m_Camera *= m_Translation;

	// Set model matrix to identity since it is assumed the models local space is world space
	m_Model = glm::mat4(1.0);

	// View Matrix is the inverse of the camera matrix
	m_View = glm::inverse(m_Camera);

	// Utils::PrintMat4(m_Camera);

	m_VertFov = 2 * atan(tan(m_HorFOV / 2)*(m_ImageHeight / m_ImageWidth));

	m_Projection = glm::perspective(
		m_VertFov,						// vertical FOV
		m_ImageWidth / m_ImageHeight,   // Aspect ratio
		0.1f,						    // near clipping plane
		100.0f					        // far clipping plane
	);
	
	glm::mat4 modelView = m_View * m_Model;

	m_Normal = glm::transpose(glm::inverse(modelView));

	m_MVP = m_Projection * modelView;
}

void Camera::PrintCameraContents()
{
	std::cout << "Translation / Position: " << m_Position.x << "," << m_Position.y << "," << m_Position.z << std::endl;
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
