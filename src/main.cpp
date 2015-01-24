#include <iostream>
#include <SFML/Graphics.hpp>
#include <bitset>
#include "SceneManager.hpp"

// define number of keyboards binding 
#define INPUT_SIZE 5
typedef std::bitset<INPUT_SIZE> InputType;
static sf::Keyboard::Key keyboardBinding[INPUT_SIZE] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::A
};

static sf::RenderWindow window(sf::VideoMode(800, 600), "Galactic Irrweg");
static SceneManager sceneManager;

static bool focus = true;
static InputType input;

int main() {
	// window settings
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30); // avoid noise ;)
	
	// define a clock to measure time
	sf::Clock clock;
	sf::Time deltaT = clock.restart();
	
	// main loop
	while (window.isOpen()) {
		// poll events (do not use for input handling)
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::LostFocus) {
				focus = false;
			}
			if (event.type == sf::Event::GainedFocus) {
				focus = true;
			}
		}
		
		// retrieve current input
		for (int i = 0; i < INPUT_SIZE; ++i) {
			input[i] = sf::Keyboard::isKeyPressed(keyboardBinding[i]) && focus;
			std::cout << i << std::endl;
		}
		// clear window content
		window.clear();
		// reset clock and determine elapsed time since last frame
		deltaT = clock.restart();
		// TODO update game with deltaT when focus
		if (focus) {
			sceneManager.update(deltaT);
		}
		window.display();
	}
}
