#include <iostream>  // remove afterwards, using for cout
#include <fstream>
#include "JSONLoader.h"


JSONLoader::JSONLoader()
{
}

void JSONLoader::ReadFromJSONFile(std::string filepath)
{
	std::string currentLine;
	
	std::ifstream JSONFile;
	JSONFile.open(filepath);

	if (JSONFile.is_open())
	{
		while (!JSONFile.eof())
		{
			std::getline(JSONFile, currentLine);
			std::cout << currentLine << std::endl;
		}
		
	}

	


	//int main() {
	//	ofstream myfile;
	//	myfile.open("example.txt");
	//	myfile << "Writing this to a file.\n";
	//	myfile.close();
	//	return 0;
}


