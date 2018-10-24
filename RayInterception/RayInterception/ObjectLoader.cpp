#include "ObjectLoader.h"
#include "ObjectData.h"

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <functional>
#include <fstream>

#define MAX_PATH 260

ObjectDataPtr ObjectLoader::Load(const std::string& objPath)
{
	/////////////////
	// open file. //
	///////////////

	std::ifstream file(objPath);
	std::string line;
	LineElements element;

	//////////////////////
	// get array sizes //
	////////////////////
	uint64_t positionCount = 0;
	uint64_t textureUVCount = 0;
	uint64_t normalCount = 0;
	uint64_t faceCount = 0;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.size() > 2 && line[0] == 'v')
			{
				if (line[1] == ' ') 
				{
					++positionCount;
				}
				else if (line[1] == 't') 
				{
					++textureUVCount;
				}
				else if (line[1] == 'n') 
				{
					++normalCount; 
				}
			}
			else if (line.size() > 1 && line[0] == 'f') 
			{
				++faceCount; 
			}
		}
	}
	else
	{
		return nullptr;
	}

	std::vector< glm::vec3 > positions(positionCount);
	std::vector< glm::vec2 > textureUVs(textureUVCount);
	std::vector< glm::vec3 > normals(normalCount);

	auto pData = std::make_shared<ObjectData>(faceCount);

	positionCount = 0;
	textureUVCount = 0;
	normalCount = 0;
	faceCount = 0;
	uint64_t vertexIndex = 0;

	/////////////////////////
	// File loading logic //
	///////////////////////
	std::vector< std::function< void(const std::string&) > > translator(element_count);

	translator[v] = [&positions, &positionCount](const std::string& str)
	{
		glm::vec3& position = positions[positionCount];	
		//This may look incorrect but it's correct as models are created using a different co-ordinate system that tracking system  
		sscanf_s(str.c_str(), "v %f %f %f", &position.y, &position.x, &position.z); 
		position.x *= -1;//invert the x 
		++positionCount;
	};

	translator[vt] = [&textureUVs, &textureUVCount](const std::string& str)
	{
		glm::vec2& textureUV = textureUVs[textureUVCount];
		sscanf_s(str.c_str(), "vt %f %f", &textureUV.x, &textureUV.y);
		textureUV.y = 1 - textureUV.y; // Invert the texture coord.
		++textureUVCount;
	};

	translator[vn] = [&normals, &normalCount](const std::string& str)
	{
		glm::vec3& normal = normals[normalCount];
		sscanf_s(str.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z); 
		++normalCount;
	};

	translator[f] = [&faceCount, &pData, &vertexIndex, &positions, &textureUVs, &normals](const std::string& str)
	{
		char _1[MAX_PATH] = "";
		char _2[MAX_PATH] = "";
		char _3[MAX_PATH] = "";
		sscanf_s(str.c_str(), "f %s %s %s", _1, MAX_PATH, _2, MAX_PATH, _3, MAX_PATH);

		int vIndices1[3];
		int vIndices2[3];
		int vIndices3[3];

		if (9 == sscanf_s(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
			&vIndices1[0], &vIndices1[1], &vIndices1[2],
			&vIndices2[0], &vIndices2[1], &vIndices2[2],
			&vIndices3[0], &vIndices3[1], &vIndices3[2]
		))
		{
		}
		else if (6 == sscanf_s(str.c_str(), "f %d//%d %d//%d %d//%d",
			&vIndices1[0], &vIndices1[2],
			&vIndices2[0], &vIndices2[2],
			&vIndices3[0], &vIndices3[2]
		))
		{
			vIndices1[1] = 0;
			vIndices2[1] = 0;
			vIndices3[1] = 0;
		}
		else if (6 == sscanf_s(str.c_str(), "f %d/%d/ %d/%d/ %d/%d/",
			&vIndices1[0], &vIndices1[1],
			&vIndices2[0], &vIndices2[1],
			&vIndices3[0], &vIndices3[1]
		))
		{
			vIndices1[2] = 0;
			vIndices2[2] = 0;
			vIndices3[2] = 0;
		}
		else if (3 == sscanf_s(str.c_str(), "f %d// %d// %d//",
			&vIndices1[0],
			&vIndices2[0],
			&vIndices3[0]
		))
		{
			vIndices1[1] = 0; vIndices1[2] = 0;
			vIndices2[1] = 0; vIndices2[2] = 0;
			vIndices3[1] = 0; vIndices3[2] = 0;
		}
		else
		{
			vIndices1[0] = 0; vIndices1[1] = 0; vIndices1[2] = 0;
			vIndices2[0] = 0; vIndices2[1] = 0; vIndices2[2] = 0;
			vIndices3[0] = 0; vIndices3[1] = 0; vIndices3[2] = 0;
		}

		VertexCache& vCashe1 = pData->vertexCache[_1];
		if (vCashe1.index == -1)
		{
			vCashe1.index = (uint64_t)pData->vertexCache.size() - 1;

			if (vIndices1[0] > 0) { vCashe1.vertex.pos = positions[vIndices1[0] - 1]; }
			if (vIndices1[1] > 0) { vCashe1.vertex.tex = textureUVs[vIndices1[1] - 1]; }
			if (vIndices1[2] > 0) { vCashe1.vertex.nrm = normals[vIndices1[2] - 1]; }
		}

		VertexCache& vCashe2 = pData->vertexCache[_2];
		if (vCashe2.index == -1)
		{
			vCashe2.index = (uint64_t)pData->vertexCache.size() - 1;

			if (vIndices2[0] > 0) { vCashe2.vertex.pos = positions[vIndices2[0] - 1]; }
			if (vIndices2[1] > 0) { vCashe2.vertex.tex = textureUVs[vIndices2[1] - 1]; }
			if (vIndices2[2] > 0) { vCashe2.vertex.nrm = normals[vIndices2[2] - 1]; }
		}

		VertexCache& vCashe3 = pData->vertexCache[_3];
		if (vCashe3.index == -1)
		{
			vCashe3.index = (uint64_t)pData->vertexCache.size() - 1;

			if (vIndices3[0] > 0) { vCashe3.vertex.pos = positions[vIndices3[0] - 1]; }
			if (vIndices3[1] > 0) { vCashe3.vertex.tex = textureUVs[vIndices3[1] - 1]; }
			if (vIndices3[2] > 0) { vCashe3.vertex.nrm = normals[vIndices3[2] - 1]; }
		}

		pData->indices[vertexIndex++] = vCashe3.index;
		pData->indices[vertexIndex++] = vCashe2.index;
		pData->indices[vertexIndex++] = vCashe1.index;

		++faceCount;
	};
	
	if (file.is_open())
	{
		/////////////////				
		// reset file //
		///////////////
		file.clear();
		file.seekg(0, std::ios::beg);

		///////////////
		// get data //
		/////////////
		while (getline(file, line))
		{
			element = element_count;

			if (line.size() > 2 && line[0] == 'v')
			{
				if (line[1] == ' ')
				{
					element = v;
				}
				else if (line[1] == 't') 
				{
					element = vt; 
				}
				else if (line[1] == 'n') 
				{
					element = vn;
				}
			}
			else if (line.size() > 1 && line[0] == 'f')
			{
				element = f; 
			}
			
			if (element < element_count)
			{
				translator[element](line);
			}
		}
	}
	file.close();

	/////////////////////////
	// return mesh object //
	///////////////////////
	return pData;
}