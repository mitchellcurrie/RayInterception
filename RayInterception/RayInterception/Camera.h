#pragma once
#include "JSONLoader.h"

#include <glm\glm.hpp>

class Camera 
{
public:
	Camera();
	bool InitialiseValuesFromJSON(char* JSONFilepath);
	void SetRotation();
	void SetTranslation();
	void SetMatrices();

	glm::vec4 m_Position;

	glm::mat4 m_MVP;
	glm::mat4 m_View;
	glm::mat4 m_Projection; 
	glm::mat4 m_Normal;

	float m_ImageHeight, m_ImageWidth;

private:
	glm::mat4 m_Rotation;
	glm::mat4 m_Translation;
	glm::mat4 m_Camera;
	glm::mat4 m_Model;
	
	float m_Roll, m_Pitch, m_Yaw;
	float m_FocalLength;
	float m_RadialDistortion;
	float m_HorFOV, m_VertFov;
};