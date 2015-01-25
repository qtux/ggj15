
#include "SoundManager.hpp"
#include "globals.hpp"
#include <SFML/Audio.hpp>
#include <exception>
#include <iostream>

SoundManager::~SoundManager()
{
	//std::cout<<"SoundManager says good bye!"<<std::endl;
	tidyUp();
}

void SoundManager::playMusic(std::string fileName)
{
	bkgMusic.openFromFile(fileName);
	bkgMusic.play();
}


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


void SoundManager::tidyUp()
{
	for (std::vector<sf::Sound*>::iterator sndIt = playingSound.begin(); sndIt != playingSound.end(); )
	{
		if ((*sndIt)->getStatus() == sf::Sound::Stopped) sndIt = playingSound.erase(sndIt) ; else sndIt ++;
	}
}

const sf::SoundBuffer &SoundManager::getSound(std::string sndName)
{
	if (!enableAudio) {
		return sf::SoundBuffer();
	}
	std::map<std::string, sf::SoundBuffer *>::const_iterator conIt = bufferedSounds.find(sndName);
	if (conIt == bufferedSounds.end())
	{
		loadSound(sndName);
	}
	return *bufferedSounds[sndName];
}
void SoundManager::playSound(std::string name)
{
	//std::cout<<"playSound start"<<std::endl;
	
	if (!enableAudio) {
		return;
	}
	tidyUp();
	playingSound.push_back(new sf::Sound());
	sf::Sound &snd = *(playingSound.back());
	snd.setBuffer(soundManager.getSound(name));
	//std::cout<<"playSound start"<<std::endl;
	snd.play();
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

SoundManager::SoundManager(bool enableAudio): enableAudio(enableAudio) { std::cout<<"enableAudio"<<enableAudio<<std::endl;}
