#include "global.hpp"

sf::Keyboard::Key gb::keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Space,
	sf::Keyboard::S
};

SoundManager gb::soundManager(100, 100);
TextureManager gb::textureManager;
gb::InputType gb::input;
