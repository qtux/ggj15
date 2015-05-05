#include "Player.hpp"
#include "global.hpp"
#include "Level.hpp"
#include "Tile.hpp"
#include "GameObject.hpp"

Player::Player(Level* level, const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize):
	_level(level),
	_animationStep(0.0f),
	_direction(0),
	_shape(size),
	_doggieShape(doggieSize)
{
	_shape.setTexture(&gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	_shape.setPosition(pos);
	_doggieShape.setTexture(&gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	_doggieShape.setPosition(pos);
}

bool Player::intersects(const GameObject& cmp)
{
	const sf::FloatRect &tmpRect = _shape.getGlobalBounds();
	sf::Vector2f tmpPos(tmpRect.left, tmpRect.top);
	return intersects(tmpPos, cmp);
}

bool Player::intersects(const sf::Vector2f &testPos, const GameObject& cmp)
{
	if (dynamic_cast<const Tile*>(&cmp) && dynamic_cast<const Tile*>(&cmp)->walkable) return false; // TODO: aus intersect in allgemeineren Teil verschieben
	sf::FloatRect tmpRect(testPos.x + 3, testPos.y + (32 - 10), 10, 10);
	if (cmp.mySprite == 0) return false;
	return cmp.mySprite->getGlobalBounds().intersects(tmpRect);
}

void Player::update (sf::Time deltaTime) {
	float dT = deltaTime.asSeconds();
	
	// get input from global and process:
	sf::Vector2f tmpPos = _shape.getPosition();
	sf::Vector2f oldPos(tmpPos);
	int width = _shape.getTextureRect().width;
	int height = _shape.getTextureRect().height;
	int dir = -1;
	if (!_level->textBox->enabled()){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { tmpPos.x -= 120 * dT; dir = 3; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { tmpPos.x += 120 * dT; dir = 2; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { tmpPos.y -= 120 * dT; dir = 1; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { tmpPos.y += 120 * dT; dir = 0; }
	}
	int viewWidth = gb::sizeX * gb::largeTileSizeX * gb::pixelSizeX;
	int viewHeight = gb::sizeY * gb::largeTileSizeY * gb::pixelSizeY;
	
	if (tmpPos.x + 8 > viewWidth) tmpPos.x -= viewWidth;
	if (tmpPos.x + width - 8 < 0)  tmpPos.x += viewWidth;
	if (tmpPos.y + 28 > viewHeight) tmpPos.y -= viewHeight;
	if (tmpPos.y + height - 8 < 0)  tmpPos.y += viewHeight;
	
	if (dir > -1) 
	{
		_animationStep += 8 * dT;
		_direction = dir;
	} else {
		_animationStep = 0.0f;
	}
	if (_animationStep >= 6.) _animationStep -= 6;
	
	bool collides = false;
	//check for collisions:
	int chkColl[] = {0, 0};
	for (std::vector<GameObject*>::const_iterator tileIt = _level->gameBoard.begin(); tileIt != _level->gameBoard.end(); tileIt++)
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
	for (std::vector<Item*>::iterator itIt = _level->items.begin() ; itIt != _level->items.end() ; itIt++)
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
			_positionQueue.push(tmpPos);
			_directionQueue.push(_direction);
		}
		
		_shape.setPosition(tmpPos.x, tmpPos.y);
		if (!_positionQueue.empty()){
			_doggieShape.setPosition(_positionQueue.front().x, _positionQueue.front().y + 18);
		}
	}
	
	if (!_directionQueue.empty()){
		_doggieShape.setTextureRect(sf::IntRect((_directionQueue.front() + 4) * 16, DoggieAnimState[int(_animationStep)] * 16, 16, 16));
	}
	else
	{
		_doggieShape.setTextureRect(sf::IntRect(4*16,0, 16, 16));
	}
	if (!_positionQueue.empty() && !_directionQueue.empty() && _positionQueue.size() > 0.256/dT) // delay of doggie movement
	{
		_directionQueue.pop();
		_positionQueue.pop();
	}
	
	_shape.setTextureRect(sf::IntRect(_direction * 16, PlayerAnimState[int(_animationStep)] * 32, 16, 32));
}

void Player::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	renderTarget.draw(_doggieShape, renderShader);
	renderTarget.draw(_shape, renderShader);
}
