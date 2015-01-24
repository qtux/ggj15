//SoundManager.hpp
#ifndef SOUNDMANAGER_HPP_
#define SOUNDMANAGER_HPP_

#include <map>
#include <string>
#include "AbstractManager.hpp"
#include <SFML/Audio.hpp>


class SoundManager : public AbstractManager
{
private:
	std::map<std::string, sf::SoundBuffer *> bufferedSounds;
	
public:
	void init(std::preloadFileName);
	const sf::SoundBuffer &getSound(std::string sndName);
	
}

#endif
