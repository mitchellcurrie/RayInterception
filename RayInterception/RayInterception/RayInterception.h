#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "Camera.h"
#include "ObjectLoader.h"
#include "ObjectData.h"
class RayInterception
{
public:
	RayInterception();
	static void UpdateObjectVertices(Camera _camera, ObjectDataPtr _objPtr);
	static glm::vec3 CalculateRayFromScreenPoint(int x, int y, Camera _camera);
	static void OrderVerticesBasedOnIndex(ObjectDataPtr _objPtr);
	static bool CalculateRayToObjectIntersection(glm::vec3 ray, ObjectDataPtr _objPtr, Camera _camera, glm::vec3 &intersect);
	static bool GetRayTriangleIntersection(glm::vec3 ray, glm::vec3 triIndex_1, glm::vec3 triIndex_2, glm::vec3 triIndex_3, Camera _camera, glm::vec3 &intersect);

private:
	static std::vector <VertexPosNrmTex> IndexOrderVertices;


};

