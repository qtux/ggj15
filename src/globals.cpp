#include "globals.hpp"

sf::Keyboard::Key keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Num1,
	sf::Keyboard::Num2,
	sf::Keyboard::Num3,
	sf::Keyboard::Num4,
	sf::Keyboard::Num5,
	sf::Keyboard::Num6,
	sf::Keyboard::Num7,
	sf::Keyboard::Num8,
	sf::Keyboard::Num9,
	sf::Keyboard::Num0,
	sf::Keyboard::A
};


sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Galactic Irrweg");
SceneManager sceneManager;
TextureManager textureManager;
#ifndef _WIN32
	SoundManager soundManager;
#else
	SoundManager soundManager(false);
#endif

bool focus = true;
InputType input;
sf::Clock globalClock;
