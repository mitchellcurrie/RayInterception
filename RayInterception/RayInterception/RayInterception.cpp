#include <map>
#include "RayInterception.h"
#include "ObjectData.h"

RayInterception::RayInterception()
{
}

void RayInterception::UpdateObjectVertices(Camera _camera, ObjectDataPtr _objPtr)
{
	std::unordered_map<std::string, VertexCache>::iterator it;

	for (it = _objPtr->vertexCache.begin(); it != _objPtr->vertexCache.end(); it++)
	{
		glm::vec4 temp{ it->second.vertex.pos[0], it->second.vertex.pos[1], it->second.vertex.pos[2], 1 };
		temp = _camera.m_MVP * temp;
		it->second.vertex.pos[0] = temp[0];
		it->second.vertex.pos[1] = temp[1];
		it->second.vertex.pos[2] = temp[2];

		temp = { it->second.vertex.nrm[0], it->second.vertex.nrm[1], it->second.vertex.nrm[2], 1 };
		temp = _camera.m_Normal * temp;
		it->second.vertex.nrm[0] = temp[0];
		it->second.vertex.nrm[1] = temp[1];
		it->second.vertex.nrm[2] = temp[2];
	}
}

void RayInterception::ScreenToWorld(glm::vec2 & v)
{

}


