#pragma once
#include <string>
#include <map>

class JSONLoader
{
public:
	JSONLoader();
	static bool ReadJSONandFillMap(char* filepath);
	static std::multimap<std::string, float> m_dataMap;
};

