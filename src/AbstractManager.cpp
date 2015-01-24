#include "AbstractManager.hpp"

void AbstractManager::readResourcesFileNameList(std::string inputFileName, std::vector<std::string> &resFnList)
{
	std::ifstream tmpReadFile;
	tmpReadFile.open(inputFileName.c_str());
	if (tmpReadFile.is_open())
	{
		while (!tmpReadFile.eof())
		{
			std::string tmpSysName;
			tmpReadFile >> tmpSysName;
			if (tmpSysName.length() > 0) resFnList.push_back(tmpSysName);
		}
	}
	tmpReadFile.close();
}
