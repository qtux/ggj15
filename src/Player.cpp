#include "Player.hpp"
#include "global.hpp"
#include "Level.hpp"
#include "Tile.hpp"
#include "Item.hpp"
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
	
	bool collides = false;	// collision detection with gameboard and items temporarily disabled
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
