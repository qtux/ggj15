#include <iostream>

// last include (requires previous includes)
#include "globals.hpp"

#define DEBUG

void resize(int width, int height) {
	// get ratio based on the original size
	float widthRatio = (float) window.getSize().x / gridWidth;	// TODO add bar widths
	float heightRatio = (float) window.getSize().y / gridHeight;
	
	// use the smaller ratio to update the window size
	if (heightRatio > widthRatio) {
		// black border up and down (undesirable)
		int offset = (height - width * sizeY / sizeX) / widthRatio / 2;
		window.setView(sf::View(sf::FloatRect(0, -offset, width / widthRatio, height / widthRatio)));
		background.setPosition(0, -offset);
		// resize background
		background.setSize(sf::Vector2f(width / widthRatio, height / widthRatio));
		background.setTextureRect(sf::IntRect(0, 0, width / widthRatio, height / widthRatio));
	}
	else {
		// black border left and right
		int offset = (width - height * sizeX / sizeY) / heightRatio / 2;
		window.setView(sf::View(sf::FloatRect(-offset, 0, width / heightRatio, height / heightRatio)));
		background.setPosition(-offset, 0);
		// resize background
		background.setSize(sf::Vector2f(width / heightRatio, height / heightRatio));
		background.setTextureRect(sf::IntRect(0, 0, width / heightRatio, height / heightRatio));
	}
}

int main() {
	// window settings
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30); // avoid noise ;)
	window.setMouseCursorVisible(false);
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile(std::string(PATH) + "img/background.png");
	backgroundTexture.setRepeated(true);
	background.setTexture(&backgroundTexture);
	resize(screenWidth, screenHeight);
	
	// define a clock to measure time
	sf::Clock clock;
	sf::Time deltaT = clock.restart();
	globalClock.restart();

	soundManager.playMusic(std::string(PATH) + "sound/backgroundFast.ogg");
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
				int width = event.size.width;
				int height = event.size.height;
				resize(width, height);
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
		window.draw(background);
		if (focus) {
			sceneManager.update(deltaT);
		}
		window.display();
	}
}
