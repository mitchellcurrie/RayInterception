#pragma once
#include <glm\glm.hpp>
#include "JSONLoader.h"

class Camera 
{
public:
	Camera();
	bool InitialiseValuesFromJSON(char* JSONFilepath);
	void SetRotation();
	void SetTranslation();
	void SetMatrices();
	void PrintCameraContents(); //remove later

	glm::mat4 m_MVP; // move to public for testing
	glm::mat4 m_Normal; // testing

	float m_ImageHeight, m_ImageWidth; // keep public?

private:
	glm::vec4 m_Position;

	glm::mat4 m_Rotation;
	glm::mat4 m_Translation;
	glm::mat4 m_Camera;

	glm::mat4 m_Model;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
//	glm::mat4 m_MVP;

//	glm::mat4 m_Normal;
	

	float m_Roll, m_Pitch, m_Yaw;
	float m_FocalLength;
	float m_RadialDistortion;
	float m_HorFOV, m_VertFov;
};


//The FOV camara parameter is for setting the horizontal FOV. You get the vertical FOV by dividing with the image aspect ratio.

//Say you image is 1:1 aspect ratio, then the vertical and horizontal FOV will be the same.

//If your FOV is 2:1, then the vertical FOV is half the horizontal value.

//So, it's Horizontal FOV / aspect ratio = vertical FOV.

//If your aspect ratio is 16:9, you must multiply by 0.5625 (because 9/16=0.5625)
