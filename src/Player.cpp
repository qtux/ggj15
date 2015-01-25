#include "Player.hpp"
#include <iostream>
#include "Tile.hpp"
#include "Scene.hpp"

bool Player::intersects(const GameObject& cmp)
{
	const sf::FloatRect &tmpRect = mySprite->getGlobalBounds();
	sf::Vector2f tmpPos(tmpRect.left, tmpRect.top);
	return intersects(tmpPos, cmp);
}

bool Player::intersects(const sf::Vector2f &testPos, const GameObject& cmp)
{
	if (dynamic_cast<const Tile*>(&cmp) && dynamic_cast<const Tile*>(&cmp)->walkable) return false; // TODO: aus intersect in allgemeineren Teil verschieben
	sf::FloatRect tmpRect(testPos.x + 3 * mySprite->getScale().x, testPos.y + (32 - 10) * mySprite->getScale().y, 10* mySprite->getScale().x, 10* mySprite->getScale().y);
	
	/*
	tmpRect.top += 32 - 10;
	tmpRect.left += 3;
	tmpRect.width = 10;
	tmpRect.height = 10;*/
	
	if (cmp.mySprite == 0) return false;
	return cmp.mySprite->getGlobalBounds().intersects(tmpRect);
}

void Player::update (sf::Time deltaTime) {
	
	float dT = float(deltaTime.asMilliseconds());
	float currTime = globalClock.getElapsedTime().asSeconds();
	
	// get input from globals and process:
	sf::Vector2f tmpPos = getPosition();
	int width = getWidth();
	int height = getHeight();
	int dir = -1;
	if (input[0]) { tmpPos.x -= 0.12 * dT* (.75+.25*(sin(currTime*40)+1)); dir = 3; }
	if (input[1]) { tmpPos.x += 0.12 * dT*(.75+.25*(sin(currTime*40)+1)); dir = 2; }
	if (input[2]) { tmpPos.y -= 0.12 * dT*(.75+.25*(sin(currTime*40)+1)); dir = 1; }
	if (input[3]) { tmpPos.y += 0.12 * dT*(.75+.25*(sin(currTime*40)+1)); dir = 0; }
	
	int viewWidth = Scene::sizeX * Scene::largeTileSizeX * Tile::pixelSizeX * Scene::tileScaleFactor;
	int viewHeight = Scene::sizeY * Scene::largeTileSizeY * Tile::pixelSizeY * Scene::tileScaleFactor;
	
	if (tmpPos.x > viewWidth) tmpPos.x -= viewWidth;
	if (tmpPos.x + width < 0)  tmpPos.x += viewWidth;
	if (tmpPos.y > viewHeight) tmpPos.y -= viewHeight;
	if (tmpPos.y + height < 0)  tmpPos.y += viewHeight;
	
	
	if (dir > -1) 
	{
		animationStep += 0.08*dT / slowFactor;
		doggieStep += 0.08*dT / slowFactor;
		direction = dir;
	} else {
		animationStep = 0.;
		doggieStep = 0.;
	}
	if (animationStep >= 4.) animationStep -= 3;
	if (doggieStep >= 6.) doggieStep -= 5;
	
	bool collides = false;
	//check for collisions:

	for (std::vector<GameObject*>::const_iterator tileIt = sceneManager.getCurrentScene().gameBoard.begin(); tileIt != sceneManager.getCurrentScene().gameBoard.end(); tileIt++)
	{
		sf::Vector2f distVec = ((*tileIt)->getPosition() - getPosition());
		// ... 
		//std::cout<<(*tileIt)->mySprite->getGlobalBounds().left<<" , "<<(*tileIt)->mySprite->getGlobalBounds().top<<" , "<<mySprite->getGlobalBounds().left<<" , "<<mySprite->getGlobalBounds().left<<" , "<<std::endl;
		if (distVec.x * distVec.x + distVec.y * distVec.y < 60 * 60 && intersects(tmpPos, **tileIt)) // first condition does quick distance check, 60 is arbitrary safe distance
		{
			collides = true;
		}
	}
	if (!collides)
	{
		// doggie follows the hero
		if (dir > -1)
		{
			positionQueue.push(tmpPos);
			directionQueue.push(direction);
		}
		
		setPosition(tmpPos.x, tmpPos.y);
		if (!positionQueue.empty()){
			doggieSprite->setPosition(positionQueue.front().x, positionQueue.front().y + 18*mySprite->getScale().y);
		}
	}
	
	//std::cout<<"1/dT "<<.256/dT<<std::endl;
	if (mySprite != 0 && doggieSprite != 0)
	{		
		if (!directionQueue.empty()){
			doggieSprite->setTextureRect(sf::IntRect((directionQueue.front() + 4) * 16, DoggieAnimState[int(doggieStep)] * 16, 16, 16));
		}
		else
		{
			doggieSprite->setTextureRect(sf::IntRect(4*16,0, 16, 16));
		}
		window.draw(*doggieSprite);
		if (!positionQueue.empty() && !directionQueue.empty() && positionQueue.size() > 256./dT) // delay of doggie movement
		{
			directionQueue.pop();
			positionQueue.pop();
		}
		
		mySprite->setTextureRect(sf::IntRect(direction * 16, PlayerAnimState[int(animationStep)] * 32, 16, 32));
		window.draw(*mySprite);
	}
}
