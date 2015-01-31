#include "globals.hpp"

#ifdef FIX_TO_STRING
	#include <sstream>
	namespace std {
		std::string to_string(int value) {
			std::ostringstream ss;
			ss << value;
			return ss.str();
		}
	}
#endif

sf::Keyboard::Key gb::keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Space,
	sf::Keyboard::S
};


sf::RenderWindow gb::window(sf::VideoMode(screenWidth, screenHeight), "Galactic Irrweg");
SceneManager gb::sceneManager;
TextureManager gb::textureManager;
SoundManager gb::soundManager(100, 100);

bool gb::inMenu = true;
bool gb::showOutline = true;
gb::InputType gb::input;
sf::Clock gb::clock;
