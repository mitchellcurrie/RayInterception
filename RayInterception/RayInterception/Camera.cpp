#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
{
	m_Position = glm::vec4(0,0,0,1);
	m_Roll = 0, m_Pitch = 0, m_Yaw = 0;
	m_FocalLength = 0;
	m_RadialDistortion = 0;
	m_HorFOV = 0, m_VertFov = 0;
	m_ImageHeight = 0, m_ImageWidth = 0;
}

// Takes the information stored in the JSONLoader map and assigns them to member variables
bool Camera::InitialiseValuesFromJSON(char* JSONFilepath)
{
	// Read values from JSON file and populate map (JSONLoader::m_dataMap)
	if (!JSONLoader::ReadJSONandFillMap(JSONFilepath))
	{
		return false;
	}

	std::multimap<std::string, float> ::iterator it;

	// Get 3 translations values from map (Camera position x, y, z) - all stored in the map under the key: "translation"
	auto translationValues = JSONLoader::m_dataMap.equal_range("translation");
	int count = 0;

	for (auto i = translationValues.first; i != translationValues.second; ++i)
	{
		m_Position[count] = i->second;
		count++;
	}

	// Iterate through the map and store camera member variables based on map key
	for (it = JSONLoader::m_dataMap.begin(); it != JSONLoader::m_dataMap.end(); it++)
	{	
		if (it->first == "roll")
			m_Roll = it->second;

		else if (it->first == "pitch")
			m_Pitch = it->second;

		else if (it->first == "yaw")
			m_Yaw = it->second;

		else if (it->first == "focal length")
			m_FocalLength = it->second;

		else if (it->first == "horizontal fov")
			m_HorFOV = it->second;

		else if (it->first == "radial distortion")
			m_RadialDistortion = it->second;

		else if (it->first == "width")
			m_ImageWidth = it->second;

		else if (it->first == "height")
			m_ImageHeight = it->second;
	}

	return true;
}

void Camera::SetRotation()
{
	// Euler ZXZ Rotation Format //

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

	// Set model matrix to identity since it is assumed the model's local space is world space
	m_Model = glm::mat4(1.0);

	// View Matrix is the inverse of the camera matrix
	m_View = glm::inverse(m_Camera);

	// Calculate vertical FOV based on horizontal FOV and the image width and height.
	m_VertFov = 2 * atan(tan(m_HorFOV / 2)*(m_ImageHeight / m_ImageWidth));

	// Calculate projection matrix using glm perspective function
	m_Projection = glm::perspective(
		m_VertFov,						// vertical FOV
		m_ImageWidth / m_ImageHeight,   // Aspect ratio
		0.1f,						    // near clipping plane
		100.0f					        // far clipping plane
	);
	
	glm::mat4 modelView = m_View * m_Model;

	// Normal matrix is the transpose of the inverse model view matrix
	m_Normal = glm::transpose(glm::inverse(modelView));

	// Calculate MVP matrix to multiply with mesh vertices
	m_MVP = m_Projection * modelView;
}
