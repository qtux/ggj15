#include "Player.hpp"
#include "globals.hpp"

void Player::update (sf::Time deltaTime) {
	// get input from globals and process:
	sf::Vector2f &tmpPos = getPosition();
	int width = getWidth();
	int height = getHeight();
	if (input[0]) setPosition(tmpPos.x - 1, tmpPos.y);
	if (input[1]) setPosition(tmpPos.x + 1, tmpPos.y);
	if (input[2]) setPosition(tmpPos.x, tmpPos.y - 1);
	if (input[3]) setPosition(tmpPos.x, tmpPos.y + 1);
	
	if (tmpPos.x > screenWidth) tmpPos.x -= screenWidth;
	if (tmpPos.x + width < 0)  tmpPos.x += screenWidth;
	if (tmpPos.y > screenHeight) tmpPos.y -= screenHeight;
	if (tmpPos.y + height < 0)  tmpPos.y += screenHeight;
	
	setPosition(tmpPos.x, tmpPos.y);
	//check for collisions:
		
	
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
}
