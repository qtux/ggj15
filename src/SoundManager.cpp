#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager(float soundVolume, float musicVolume):
	bkgMusic(new sf::Music()),
	musicVolume(musicVolume),
	soundVolume(soundVolume)
{
	bkgMusic->setVolume(musicVolume);
}

SoundManager::~SoundManager() {
	// stop music and sound effects
	bkgMusic->stop();
	for (auto& sound: playingSounds) {
		sound->stop();
	}
	// delete the ressources
	for (auto& sound: playingSounds) {
		delete sound;
	}
	for (auto& buffer: bufferedSounds) {
		delete buffer.second;
	}
	delete bkgMusic;
}

void SoundManager::playSound(std::string fileName) {
	// tidy up playingSounds by deleting and removing stopped sound buffers
	for (auto it = playingSounds.begin(); it != playingSounds.end();) {
		if ((*it)->getStatus() == sf::Sound::Stopped) {
			delete (*it);
			it = playingSounds.erase(it);
		}
		else {
			++it;
		}
	}
	
	// return if the system limit is reached
	if (playingSounds.size() >= MAX_SOUND_INSTANCES) {
		return;
	}
	
	// otherwise get the sound buffer (load from file if not loaded yet)
	auto search = bufferedSounds.find(fileName);
	if (search == bufferedSounds.end()) {
		sf::SoundBuffer *buffer = new sf::SoundBuffer();
		if (!buffer->loadFromFile(fileName)) {
			delete buffer;
			return;
		}
		// hint: insert is not required here because it is already
		// checked whether the entry does exist or not
		bufferedSounds[fileName] = buffer;
	}
	
	// play the sound file and push back the pointer to destroy the sound later on
	sf::Sound* snd = new sf::Sound();
	snd->setBuffer(*(bufferedSounds[fileName]));
	snd->setVolume(soundVolume);
	snd->play();
	playingSounds.push_back(snd);
}

void SoundManager::playMusic(std::string fileName) {
	// try to load a new background music file
	if (!bkgMusic->openFromFile(fileName)) {
		return;
	}
	bkgMusic->setLoop(true);
	bkgMusic->play();
}

void SoundManager::pause() {
	// pause the background music
	bkgMusic->pause();
	// pause currently running sound effects
	for (auto& sound: playingSounds) {
		sound->pause();
	}
}

void SoundManager::resume() {
	// resume the background music if it was paused previously
	if (bkgMusic->getStatus() == sf::SoundSource::Paused) {
		bkgMusic->play();
	}
	// resume paused sound effect
	for (auto& sound: playingSounds) {
		if (sound->getStatus() == sf::SoundSource::Paused) {
			sound->play();
		}
	}
}
