#include "globals.hpp"

sf::Keyboard::Key keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::A
};


sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Galactic Irrweg");
SceneManager sceneManager;
#ifndef _WIN32
	SoundManager soundManager;
#else
	SoundManager soundManager(false);
#endif

bool focus = true;
InputType input;
sf::Clock globalClock;
