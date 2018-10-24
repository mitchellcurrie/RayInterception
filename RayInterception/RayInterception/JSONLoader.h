#pragma once
#include <string>
#include <map>

class JSONLoader
{
public:
	JSONLoader();
	void ReadFromJSONFile(std::string filepath);

private:
	std::multimap<std::string, float> m_dataMap;


};

