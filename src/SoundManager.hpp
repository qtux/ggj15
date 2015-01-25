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
	sf::Music bkgMusic;
	void loadSound(std::string sndName);
	std::vector<sf::Sound*> playingSound;
	void tidyUp();
	bool enableAudio;
public:
	void init(std::string preloadFileName);
	const sf::SoundBuffer &getSound(std::string sndName);
	SoundManager(bool enableAudio = true);
	void playMusic(std::string fileName);
	~SoundManager();
	void playSound(std::string name);
};

#endif
