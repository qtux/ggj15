#include <iostream>

// last include (requires previous includes)
#include "globals.hpp"

#define DEBUG

int main() {
	// window settings
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30); // avoid noise ;)
	
	// define a clock to measure time
	sf::Clock clock;
	sf::Time deltaT = clock.restart();
	globalClock.restart();
	
	soundManager.playSound("sound/test.wav");
	
	// main loop
	while (window.isOpen()) {
		// poll events (do not use for input handling)
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed  || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
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
		}
		
		#ifdef DEBUG
			// show input bitstring if any key is pressed
			if (input.any()) {
				std::cout << input << std::endl;
			}
		#endif
		
		// clear window content
		window.clear();
		// reset clock and determine elapsed time since last frame
		deltaT = clock.restart();
		// update game with deltaT when focused
		if (focus) {
			sceneManager.update(deltaT);
		}
		window.display();
	}
}
