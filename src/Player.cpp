#include "Player.hpp"
#include "Player.hpp"
#include "global.hpp"

// TODO test - remove the following includes
#include <iostream>
#include "TileMap.hpp"

Player::Player(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize):
	velocity(240.0f),
	_collider(pos, size),		// TODO adapt this
	_animationStep(0.0f),
	_direction(0),
	_shape(size),
	_doggieShape(doggieSize)
{
	_shape.setTexture(&gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	_shape.setPosition(pos);
	_doggieShape.setTexture(&gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	_doggieShape.setPosition(pos);
	// TODO improve following code
	_collider.top += 32;
	_collider.height -=32;
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

void Player::move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map)
// TODO add a grid of collision/gameobject data (replace map)
{
	// TODO remove oneself from the grid data
	
	// determine offset and requrested target position
	sf::Vector2f offset(moveDir * velocity * deltaTime.asSeconds());
	sf::Vector2f target(_collider.left, _collider.top);
	
	// determine grid and tile size
	const sf::Vector2i gridSize(gb::sizeX * gb::largeTileSizeX, gb::sizeY * gb::largeTileSizeY);
	const sf::Vector2i tileSize(gb::pixelSizeX, gb::pixelSizeY);
	
	// 1. try to move horizontally
	target.x += offset.x;
	// warp
	target.x -= (gridSize.x * tileSize.x) * std::floor(target.x / (gridSize.x * tileSize.x));
	// do collision resolution for every neighbour (dependend on collider size)
	for (auto i = 0; i < std::ceil(_collider.width / tileSize.x) + 1; ++i)
	{
		for (auto j = 0; j < std::ceil(_collider.height / tileSize.y) + 1; ++j)
		{
			// determine current tile position
			sf::Vector2u tilePos(i + std::floor(target.x / tileSize.x), j + std::floor(target.y / tileSize.y));
			// resolve collision if there is one
			if (map->isSolid(tilePos))
			{
				if (_collider.left < tilePos.x * tileSize.x)
				{
					target.x -= _collider.left + _collider.width - tilePos.x * tileSize.x;
				}
				else
				{
					target.x += tilePos.x * tileSize.x + 32 - _collider.left;
				}
			}
		}
	}
	
	// 2. try to move vertically
	target.y += offset.y;
	// warp
	target.y -= (gridSize.y * tileSize.y) * std::floor(target.y / (gridSize.y * tileSize.y));
	// do collision resolution for every neighbour (dependend on collider size)
	for (auto i = 0; i < std::ceil(_collider.width / tileSize.x) + 1; ++i)
	{
		for (auto j = 0; j < std::ceil(_collider.height / tileSize.y) + 1; ++j)
		{
			// determine current tile position
			sf::Vector2u tilePos(i + std::floor(target.x / tileSize.x), j + std::floor(target.y / tileSize.y));
			// resolve collision if there is one
			if (map->isSolid(tilePos))
			{
				if (_collider.top < tilePos.y * tileSize.y)
				{
					target.y -= _collider.top + _collider.height - tilePos.y * tileSize.y;
				}
				else
				{
					target.y += tilePos.y * tileSize.y + 32 - _collider.top;
				}
			}
		}
	}
	
	// TODO add oneself on the new position to the grid data
	
	// move the shape
	_collider.left = target.x;
	_collider.top = target.y;
	_shape.setPosition(target);
	
	// update animation steps and push movement data to allow doggie following the hero
	int dir = (moveDir.x < 0) ? 3 : (moveDir.x > 0) ? 2 : (moveDir.y < 0) ? 1 : (moveDir.y > 0) ? 0 : -1;
	if (dir > -1)
	{
		_animationStep += 8 * deltaTime.asSeconds();
		//_positionQueue.push(nextPos);	// TODO reenable me using offset instead of positions
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
