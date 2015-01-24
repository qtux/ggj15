#pragma once

#include <vector>
#include <string>
#include <fstream>

class AbstractManager
{
	
protected:
	void readResourcesFileNameList(std::string inputFileName, std::vector<std::string> &resFnList);
};
