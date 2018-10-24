#pragma once
#include <glm\glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

struct VertexPosNrmTex
{
	glm::vec3 pos;
	glm::vec3 nrm;
	glm::vec2 tex;
};

class VertexCache
{
public:
	VertexCache()
		: vertex()
		, index(-1)
	{ }

	VertexPosNrmTex vertex;
	uint64_t index;
};

class ObjectData
{
public:
	ObjectData(size_t faceCount)
		: vertexCache()
		, indices(faceCount * 3)
	{};

	std::unordered_map< std::string, VertexCache > vertexCache;
	std::vector< uint64_t > indices;
};

