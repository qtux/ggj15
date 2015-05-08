#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.hpp"
//#include "globals.hpp"

const sf::Vector2f& GameObject::getPosition() const
{
	return mySprite->getPosition();
}

void GameObject::setPosition(float x, float y)
{
	mySprite->setPosition(x, y);
}

void GameObject::setPosition(const sf::Vector2f &a)
{
	mySprite->setPosition(a);
}

bool GameObject::intersects(const GameObject& cmp) const
{
	return cmp.mySprite->getGlobalBounds().intersects(mySprite->getGlobalBounds());
}

void GameObject::checkTilesCollision(sf::Vector2f& tmpPos,
		const sf::Vector2f& oldPos, int chkColl[2]) {
	for (std::vector<GameObject*>::const_iterator tileIt = level->gameBoard.begin(); tileIt != level->gameBoard.end(); tileIt++)
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
}


int GameObject::getHeight() const
{
	const sf::IntRect& tmp = mySprite->getTextureRect();
	return int(tmp.height * mySprite->getScale().y);
}

int GameObject::getWidth() const
{
	const sf::IntRect& tmp = mySprite->getTextureRect();
	return int(tmp.width * mySprite->getScale().x);
}

bool GameObject::intersects(const sf::Vector2f& testPos,
		const GameObject& cmp) const {
	return cmp.mySprite->getGlobalBounds().contains(testPos);
}
