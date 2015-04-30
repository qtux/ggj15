#include "global.hpp"

sf::Keyboard::Key gb::keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Space,
	sf::Keyboard::S
};


sf::RenderWindow gb::window(sf::VideoMode(screenWidth, screenHeight), "Galactic Irrweg");
SoundManager gb::soundManager(100, 100);
// input manager
TextureManager gb::textureManager;
SceneManager gb::sceneManager;

bool gb::inMenu = true;
gb::InputType gb::input;
sf::Clock gb::clock;
