#pragma once
#include <glm\glm.hpp>
#include "Camera.h"
#include "ObjectLoader.h"
class RayInterception
{
public:
	RayInterception();
	static void UpdateObjectVertices(Camera _camera, ObjectDataPtr _objPtr);
	static void ScreenToWorld(glm::vec2 &v);

private:
	


};

