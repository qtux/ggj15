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

	soundManager.playMusic("sound/backgroundFast.ogg");
	//soundManager.playSound("sound/test.wav");
	
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
			// maintain aspect ratio
			if (event.type == sf::Event::Resized) {
				// get ratio based on the original size
				float widthRatio = (float) window.getSize().x / screenWidth;
				float heightRatio = (float) window.getSize().y / screenHeight;
				// use the smaller ratio to update the window size
				//window.waitEvent(sf::Event::Resized);
				if (heightRatio > widthRatio) {
					window.setView(sf::View(sf::FloatRect(0, 0, event.size.width / widthRatio, event.size.height / widthRatio)));
				}
				else {
					window.setView(sf::View(sf::FloatRect(0, 0, event.size.width / heightRatio, event.size.height / heightRatio)));
				}
			}
		}
		

		// retrieve input (either gamepad or keyboard)
		if (sf::Joystick::isConnected(0)) {
			// retrieve current gamepad input
			input[0] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == -100;
			input[1] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == 100;
			input[2] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == -100;
			input[3] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == 100;
			// TODO input 4 - 7 buggy?
			for (int i = 0; i < 3; ++i) {
				input[i + 4] = sf::Joystick::isButtonPressed(0, i);
			}
		}
		else {
			// retrieve current keyboard input
			for (int i = 0; i < INPUT_SIZE; ++i) {
				input[i] = sf::Keyboard::isKeyPressed(keyboardBinding[i]) && focus;
			}
		}
		
		#ifdef DEBUG
			// show input bitstring if any key is pressed
			if (input.any()) {
//				std::cout << input << std::endl;
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
