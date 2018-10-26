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
		// Convert position vec3 to vec4, adding 1 at the end
		glm::vec4 temp{ it->second.vertex.pos, 1 };
		// Multiply position by MVP matrix
		temp = _camera.m_MVP * temp;
		// Assign the new position back to the vertex cache
		it->second.vertex.pos = temp;

		// Same calculations for the normal, multiplying by the normal matrix
		temp = { it->second.vertex.nrm, 1 };
		temp = _camera.m_Normal * temp;
		it->second.vertex.nrm = temp;
	}
}

void RayInterception::ScreenToWorld(glm::vec2 & v, Camera _camera)
{
	// X and Y are in pixels here
	float x = v[0];
	float y = v[1];
	
	// Translate x and y to move origin to centre of view port
	x -= _camera.m_ImageWidth / 2;
	y -= _camera.m_ImageHeight / 2;

	// Scale x and y to the range of -1 to 1
	x /= (_camera.m_ImageWidth / 2);
	y /= (_camera.m_ImageHeight / 2);



	v[0] = x;
	v[1] = y;
}


