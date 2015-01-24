
#include "SoundManager.hpp"
#include "config.hpp"
#include <SFML/Audio.hpp>
#include <exception>

void SoundManager::loadSound(std::string sndName)
{
	if (!enableAudio)
	{
		return;
	}
	sf::SoundBuffer *myBuf = new sf::SoundBuffer();
	if (!myBuf->loadFromFile(sndName))
	{
		throw std::runtime_error("Sound file not found!");
	}
	bufferedSounds.insert(std::pair<std::string, sf::SoundBuffer*>(sndName, myBuf));
}


const sf::SoundBuffer &SoundManager::getSound(std::string sndName)
{
	std::map<std::string, sf::SoundBuffer *>::const_iterator conIt = bufferedSounds.find(sndName);
	if (conIt == bufferedSounds.end())
	{
		loadSound(sndName);
	}
	return *bufferedSounds[sndName];
}


void SoundManager::init(std::string preloadFileName)
{
	if (!enableAudio)
	{
		return;
	}
	bufferedSounds.clear(); // doesn't tidy up!
	std::vector<std::string> loadFnList;
	readResourcesFileNameList(preloadFileName, loadFnList);
	for (std::vector<std::string>::iterator ldIt = loadFnList.begin(); ldIt != loadFnList.end(); ldIt++)
	{
		loadSound(*ldIt);
	}
}
