#include "Player.hpp"
#include "global.hpp"
#include "TileMap.hpp"

#include <iostream>

Player::Player(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize):
	Entity(240.0f, sf::IntRect(pos.x, pos.y + 32, size.x, size.y - 32)),	// TODO temporary - define instead a proper collider
	_animationStep(0.0f),
	_direction(0),
	_shape(size),
	_doggieShape(doggieSize)
{
	_shape.setTexture(&gb::ressourceManager.getTexture(std::string(PATH) + "img/player.png", false));
	_shape.setPosition(pos);
	_doggieShape.setTexture(&gb::ressourceManager.getTexture(std::string(PATH) + "img/player.png", false));
	_doggieShape.setPosition(pos);
}

void Player::move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map)
{
	// call parent class move
	Entity::move(deltaTime, moveDir, sceneSize, map);
	
	// move the shape according to the colliders position
	_shape.setPosition(_collider.left, _collider.top - 32);
	
	// update animation steps and push movement data to allow doggie following the hero
	int dir = (moveDir.x < 0) ? 3 : (moveDir.x > 0) ? 2 : (moveDir.y < 0) ? 1 : (moveDir.y > 0) ? 0 : -1;
	if (dir > -1)
	{
		_animationStep += 8 * deltaTime.asSeconds();
		_positionQueue.push(_shape.getPosition());
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
