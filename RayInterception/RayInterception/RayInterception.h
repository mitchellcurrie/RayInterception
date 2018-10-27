#pragma once
#include <glm\glm.hpp>
#include "Camera.h"
#include "ObjectLoader.h"
class RayInterception
{
public:
	RayInterception();
	static void UpdateObjectVertices(Camera _camera, ObjectDataPtr _objPtr);
	static glm::vec3 CalculateRayFromScreenPoint(float x, float y, Camera _camera);

private:
	


};

