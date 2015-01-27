#include "globals.hpp"

sf::Keyboard::Key global::keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Space,
	sf::Keyboard::S
};


sf::RenderWindow global::window(sf::VideoMode(screenWidth, screenHeight), "Galactic Irrweg");
SceneManager global::sceneManager;
TextureManager global::textureManager;
#ifndef _WIN32
	SoundManager global::soundManager;
#else
	SoundManager global::soundManager(false);
#endif

bool global::inMenu = true;
bool global::showOutline = true;
global::InputType global::input;
sf::Clock global::clock;
