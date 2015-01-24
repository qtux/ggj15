#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.hpp"

int main() {
	// window settings
	sf::RenderWindow window(sf::VideoMode(800, 600), "Galactic Irrweg");
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30); // avoid noise ;)
	
	bool focus = true;
	// define a clock to measure time
	sf::Clock clock;
	sf::Time deltaT = clock.restart();;
	
	Game game(&window);
	
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
		window.clear();
		deltaT = clock.restart();
		if (focus) {
			game.update(deltaT);
		}
		
		window.display();
	}
}
