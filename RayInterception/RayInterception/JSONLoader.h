#pragma once
#include <string>
#include <map>

class JSONLoader
{
public:
	JSONLoader();
	static void PrintMap(); // delete later and from cpp file
	static bool ReadJSONandFillMap(char* filepath);
	static std::multimap<std::string, float> m_dataMap;
};

