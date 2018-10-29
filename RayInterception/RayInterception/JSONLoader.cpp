#include "JSONLoader.h"

#include <fstream>
#include <sstream>

std::multimap<std::string, float> JSONLoader::m_dataMap;

JSONLoader::JSONLoader()
{
}

bool JSONLoader::ReadJSONandFillMap(char* filepath)
{
	std::string currentLine = "";
	std::string currentKey = "";
	
	std::ifstream JSONFile;
	JSONFile.open(filepath);

	if (!JSONFile.is_open())
	{
		printf("ERROR: Could not open file in ReadJSONandFillMap.\n");
		return false;
	}

	while (!JSONFile.eof())
	{
		std::getline(JSONFile, currentLine);

		std::size_t firstFoundQuote = currentLine.find_first_of("\"");
			
		if (firstFoundQuote != std::string::npos)
		{
			currentKey = currentLine.substr(firstFoundQuote + 1, currentLine.find_last_of("\"") - 1 - firstFoundQuote);
		}

		std::size_t firstFoundDigit = currentLine.find_first_of("-0123456789");

		if (firstFoundDigit != std::string::npos)
		{
			std::string value = currentLine.substr(firstFoundDigit, currentLine.find_last_of("-0123456789") - firstFoundDigit + 1);
			m_dataMap.insert(std::make_pair(currentKey, stof(value)));
		}			
	}		
	
	JSONFile.close();
	return true;
}


