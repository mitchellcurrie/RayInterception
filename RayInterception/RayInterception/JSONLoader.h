#pragma once
#include <string>
#include <map>

class JSONLoader
{
public:
	JSONLoader();
	void PrintMap(); // delete later and from cpp file
	void ReadJSONandFillMap(std::string filepath);

private:
	std::multimap<std::string, float> m_dataMap;


};

