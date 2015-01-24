#include "Player.hpp"
#include <iostream>

void Player::update (sf::Time deltaTime) {
	
	// get input from globals and process:
	sf::Vector2f tmpPos = getPosition();
	int width = getWidth();
	int height = getHeight();
	int dir = -1;
	if (input[0]) { tmpPos.x -= 0.08f*deltaTime.asMilliseconds(); dir = 3; }
	if (input[1]) { tmpPos.x += 0.08f*deltaTime.asMilliseconds(); dir = 2; }
	if (input[2]) { tmpPos.y -= 0.08f*deltaTime.asMilliseconds(); dir = 1; }
	if (input[3]) { tmpPos.y += 0.08f*deltaTime.asMilliseconds(); dir = 0; }
	
	// doggie follows the hero
	if (dir > -1)
	{
		positionQueue.push(tmpPos);
		directionQueue.push(direction);
	}
	
	if (tmpPos.x > screenWidth) tmpPos.x -= screenWidth;
	if (tmpPos.x + width < 0)  tmpPos.x += screenWidth;
	if (tmpPos.y > screenHeight) tmpPos.y -= screenHeight;
	if (tmpPos.y + height < 0)  tmpPos.y += screenHeight;
	
	setPosition(tmpPos.x, tmpPos.y);
	if (!positionQueue.empty()){
		doggieSprite->setPosition(positionQueue.front().x, positionQueue.front().y + 18);
	}
	if (dir > -1) 
	{
		animationStep += 1;
		doggieStep += 1;
		direction = dir;
	} else {
		animationStep = 0;
		doggieStep = 0;
	}
	if (animationStep / slowFactor > 3) animationStep = 0; // animationStep wird immer um 1 hochgezählt, aber effektiv um den Faktor slowFactor verlangsamt
	if (doggieStep / slowFactor > 5) doggieStep = 0;
	
	//check for collisions:
	
	
	if (mySprite != 0 && doggieSprite != 0)
	{		
		if (!directionQueue.empty()){
			doggieSprite->setTextureRect(sf::IntRect((directionQueue.front() + 4) * 16, DoggieAnimState[doggieStep / slowFactor] * 16, 16, 16));
		}
		else
		{
			doggieSprite->setTextureRect(sf::IntRect(4*16,0, 16, 16));
		}
		window.draw(*doggieSprite);
		if (!positionQueue.empty() && !directionQueue.empty() && positionQueue.size() > 16) // delay of doggie movement
		{
			directionQueue.pop();
			positionQueue.pop();
		}
		
		mySprite->setTextureRect(sf::IntRect(direction * 16, PlayerAnimState[animationStep / slowFactor] * 32, 16, 32));
		window.draw(*mySprite);
	}
}
