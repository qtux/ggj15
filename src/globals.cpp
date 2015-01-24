#include "globals.hpp"

sf::Keyboard::Key keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::A
};

sf::RenderWindow window(sf::VideoMode(800, 600), "Galactic Irrweg");
SceneManager sceneManager;
SoundManager soundManager;

bool focus = true;
InputType input = {};
