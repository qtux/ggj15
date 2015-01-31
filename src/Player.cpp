#include "Player.hpp"
#include <iostream>
#include "Tile.hpp"
#include "Scene.hpp"
#include <math.h>

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
	float dT = deltaTime.asSeconds();
	float currTime = gb::clock.getElapsedTime().asSeconds();
	
	// get input from globals and process:
	sf::Vector2f tmpPos = getPosition();
	sf::Vector2f oldPos(tmpPos);
	int width = getWidth();
	int height = getHeight();
	int dir = -1;
	if (!gb::sceneManager.getCurrentScene().textBox->enabled()){
		if (gb::input[0]) { tmpPos.x -= 120 * dT* (.75+.25*fabs(sin(currTime*30))); dir = 3; }
		if (gb::input[1]) { tmpPos.x += 120 * dT*(.75+.25*fabs(sin(currTime*30))); dir = 2; }
		if (gb::input[2]) { tmpPos.y -= 120 * dT*(.75+.25*fabs(sin(currTime*30))); dir = 1; }
		if (gb::input[3]) { tmpPos.y += 120 * dT*(.75+.25*fabs(sin(currTime*30))); dir = 0; }
	}
	int viewWidth = gb::sizeX * gb::largeTileSizeX * gb::pixelSizeX;
	int viewHeight = gb::sizeY * gb::largeTileSizeY * gb::pixelSizeY;
	
	if (tmpPos.x + 8 > viewWidth) tmpPos.x -= viewWidth;
	if (tmpPos.x + width - 8 < 0)  tmpPos.x += viewWidth;
	if (tmpPos.y + 28*mySprite->getScale().y > viewHeight) tmpPos.y -= viewHeight;
	if (tmpPos.y + height - 8  *mySprite->getScale().y < 0)  tmpPos.y += viewHeight;
	
	
	if (dir > -1) 
	{
		animationStep += 120 * dT / slowFactor;
		doggieStep += 120 * dT / slowFactor;
		direction = dir;
	} else {
		animationStep = 0.;
		doggieStep = 0.;
	}
	if (animationStep >= 6.) animationStep -= 6;
	if (doggieStep >= 6.) doggieStep -= 6;
	
	bool collides = false;
	//check for collisions:
	int chkColl[] = {0, 0};
	for (std::vector<GameObject*>::const_iterator tileIt = gb::sceneManager.getCurrentScene().gameBoard.begin(); tileIt != gb::sceneManager.getCurrentScene().gameBoard.end(); tileIt++)
	{
		sf::Vector2f distVec = ((*tileIt)->getPosition() - tmpPos);
		sf::Vector2f distVecOld = ((*tileIt)->getPosition() - oldPos); // check also neighbours of old position in case screen was left on one side
		// ... 
		//std::cout<<(*tileIt)->mySprite->getGlobalBounds().left<<" , "<<(*tileIt)->mySprite->getGlobalBounds().top<<" , "<<mySprite->getGlobalBounds().left<<" , "<<mySprite->getGlobalBounds().left<<" , "<<std::endl;
		if (distVec.x * distVec.x + distVec.y * distVec.y < 60 * 60 || distVecOld.x * distVecOld.x + distVecOld.y * distVecOld.y < 60 * 60) // && intersects(tmpPos, **tileIt)) // first condition does quick distance check, 60 is arbitrary safe distance
		{
			sf::Vector2f testPos(tmpPos);	// run additional tests for 1D movements if 2D movement is not possible
			testPos.x = oldPos.x;
			if (intersects(testPos, **tileIt)) 
			{
				chkColl[1] ++;
			}
			
			
			testPos.x = tmpPos.x;
			testPos.y = oldPos.y;
			if(intersects(testPos, **tileIt))
			{
				chkColl[0] ++;
			}
		}
	}
	if (chkColl[0] > 0)
	{
		tmpPos.x = oldPos.x;
	}
	if (chkColl[1] > 0)
	{
		tmpPos.y = oldPos.y;
	}
	for (std::vector<Item*>::iterator itIt = gb::sceneManager.getCurrentScene().items.begin() ; itIt != gb::sceneManager.getCurrentScene().items.end() ; itIt++)
	{
		if ((*itIt)->blocksPath && intersects(tmpPos, **itIt))
		{
			sf::Vector2f testPos(tmpPos);	// run additional tests for 1D movements if 2D movement is not possible
			testPos.x = oldPos.x;
			if (!intersects(testPos, **itIt)) 
			{
				tmpPos = testPos;
			}
			else
			{
				testPos.x = tmpPos.x;
				testPos.y = oldPos.y;
				if(!intersects(testPos, **itIt))
				{
					tmpPos = testPos;
				}
				else
				{
					collides = true;
				}
			}
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
		gb::window.draw(*doggieSprite);
		if (!positionQueue.empty() && !directionQueue.empty() && positionQueue.size() > 0.256/dT) // delay of doggie movement
		{
			directionQueue.pop();
			positionQueue.pop();
		}
		
		mySprite->setTextureRect(sf::IntRect(direction * 16, PlayerAnimState[int(animationStep)] * 32, 16, 32));
		gb::window.draw(*mySprite);
	}
}
