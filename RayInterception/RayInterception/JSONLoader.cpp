#include "JSONLoader.h"

#include <fstream>

std::multimap<std::string, float> JSONLoader::m_dataMap;

JSONLoader::JSONLoader()
{
}

// Populates the member variable map with the information from the JSON file
// Map key = variable name, Map value = variable value
bool JSONLoader::ReadJSONandFillMap(char* filepath)
{
	// Reads JSON file at the filepath specified
	// Stores the result in a map with the variable name as the key, and value as the value.
	
	std::string currentLine = "";
	std::string currentKey = "";
	
	// Open file
	std::ifstream JSONFile;
	JSONFile.open(filepath);

	if (!JSONFile.is_open())
	{
		printf("ERROR: Could not open file in ReadJSONandFillMap.\n");
		return false;
	}

	// Read each line and store keys and values in map
	while (!JSONFile.eof())
	{
		std::getline(JSONFile, currentLine);

		// Quotation marks surround the variable name (key)
		std::size_t firstFoundQuote = currentLine.find_first_of("\"");
			
		if (firstFoundQuote != std::string::npos)
		{
			// Store the word enclosed by the quotation marks as the current key
			currentKey = currentLine.substr(firstFoundQuote + 1, currentLine.find_last_of("\"") - 1 - firstFoundQuote);
		}

		// Digits represent the values
		std::size_t firstFoundDigit = currentLine.find_first_of("-0123456789");

		if (firstFoundDigit != std::string::npos)
		{
			std::string value = currentLine.substr(firstFoundDigit, currentLine.find_last_of("-0123456789") - firstFoundDigit + 1);

			//Insert map item based on the current key and the value
			m_dataMap.insert(std::make_pair(currentKey, stof(value)));
		}			
	}		
	
	JSONFile.close();
	return true;
}


