#include "Player.hpp"
#include "Player.hpp"
#include "global.hpp"

Player::Player(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize):
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

bool Player::intersects(const sf::Vector2u& tilePosition, const sf::Vector2f& tileSize)
{
	// TODO improve code
	sf::FloatRect playerRect = _shape.getGlobalBounds();
	playerRect.height /= 2;
	playerRect.top += playerRect.height;
	sf::Vector2f tileCoord(tilePosition.x * tileSize.x, tilePosition.y * tileSize.y);
	sf::FloatRect tileRect(tileCoord, tileSize);
	return playerRect.intersects(tileRect);
}

void Player::move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize)
{
	//sf::Vector2f offset = moveDir * 240.0f * deltaTime.asSeconds();
	sf::Vector2f offset = moveDir;
	// update player position
	sf::Vector2f nextPos = _shape.getPosition() + offset;
	//sf::Vector2f size = _shape.getSize();
	
	// check for level wrap
	/*if (nextPos.x > sceneSize.x - 8)
	{
		nextPos.x -= sceneSize.x;
	}
	if (nextPos.x < 8 - size.x)
	{
		nextPos.x += sceneSize.x;
	}
	if (nextPos.y > sceneSize.y - 28)
	{
		nextPos.y -= sceneSize.y;
	}
	if (nextPos.y < 8 - size.y)
	{ 
		nextPos.y += sceneSize.y;
	}*/
	// set new position
	//_shape.setPosition(nextPos);
	_shape.setPosition(moveDir);
	
	// doggie follows the hero and update animationStep while it is moving
	int dir = (offset.x < 0) ? 3 : (offset.x > 0) ? 2 : (offset.y < 0) ? 1 : (offset.y > 0) ? 0 : -1;
	if (dir > -1)
	{
		_animationStep += 8 * deltaTime.asSeconds();
		_positionQueue.push(nextPos);
		_directionQueue.push(_direction);
		_direction = dir;
	}
	else
	{
		_animationStep = 0.0f;
	}
}

void Player::update(sf::Time deltaTime) {
	_shape.setTextureRect(sf::IntRect(_direction * 16, PlayerAnimState[int(_animationStep) % 6] * 32, 16, 32));
	int front = (_directionQueue.empty()) ? 0 : _directionQueue.front();		// avoid UB
	_doggieShape.setTextureRect(sf::IntRect((front + 4) * 16, DoggieAnimState[int(_animationStep) % 6] * 16, 16, 16));
	
	// move doggie
	if (!_positionQueue.empty())
	{
		_doggieShape.setPosition(_positionQueue.front().x, _positionQueue.front().y + 18);
	}
	
	// dunno
	if (!_positionQueue.empty() && !_directionQueue.empty() && _positionQueue.size() > 0.256 / deltaTime.asSeconds()) // delay of doggie movement
	{
		_directionQueue.pop();
		_positionQueue.pop();
	}
}

void Player::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	renderTarget.draw(_doggieShape, renderShader);
	renderTarget.draw(_shape, renderShader);
}
