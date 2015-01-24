#include "GUI.hpp"
#include "globals.hpp"

void GUI::update (sf::Time deltaTime) {
	sf::Int32 currTime = globalClock.getElapsedTime().asMilliseconds();
	
	
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
}
