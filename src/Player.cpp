#include "Player.hpp"
#include "Player.hpp"
#include "global.hpp"

// TODO test - remove the following includes
#include <iostream>
#include "TileMap.hpp"

Player::Player(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize):
	velocity(240.0f),
	_colliderPos(pos),// TODO adapt this
	_colliderSize(size),// TODO adapt this
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
	/*_colliderPos.y += 32;
	_colliderSize.y -=32;*/
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

// TODO use integer positions (do not use unsigned integer)
// TODO add a grid of collision/gameobject data (replace map)
// TODO add a method to move more than one tile wide
void Player::move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map)
{
	// define lambdas for cleaner code
	auto mod = [] (int x, int y) {return (x < 0) ? x % y + y : x % y;};
	// determine grid and tile size TODO move elsewhere?
	sf::Vector2i gridSize(gb::sizeX * gb::largeTileSizeX, gb::sizeY * gb::largeTileSizeY);
	sf::Vector2i tileSize(gb::pixelSizeX, gb::pixelSizeY);
	
	// TODO remove oneself from the grid data
	
	// determine an integer offset (avoid jittery movements)
	sf::Vector2i offset;
	offset.x = std::round(moveDir.x * velocity * deltaTime.asSeconds());
	offset.y = std::round(moveDir.y * velocity * deltaTime.asSeconds());
	//  set target position to the current positon
	sf::Vector2i target = _colliderPos;
	
	// 1. try to move horizontally
	target.x += offset.x;
	// do collision resolution for every neighbour (dependend on collider size)
	for (int i = 0; i <= std::ceil(_colliderSize.x / tileSize.x); ++i)
	{
		for (int j = 0; j <= std::ceil(_colliderSize.y / tileSize.y); ++j)
		{
			// determine current tile position
			sf::Vector2i tilePos(i, j);
			//std::cout << tilePos.x + target.x / tileSize.x << " | " << tilePos.y + target.y / tileSize.y << std::endl;
			tilePos.x += mod(target.x / tileSize.x, gridSize.x);		// asume integer division
			tilePos.y += mod(target.y / tileSize.y, gridSize.y);		// asume integer division
			//std::cout << tilePos.x << " | " << tilePos.y << std::endl;
			// resolve collision if there is one
			if (map->isSolid(tilePos))
			{
				// if target is left compared to the current tile
				if (target.x < tilePos.x * tileSize.x)
				{
					target.x = tilePos.x * tileSize.x - _colliderSize.x;
				}
				// if target is right compared to the current tile
				else
				{
					target.x = tilePos.x * tileSize.x + tileSize.x;
				}
			}
		}
	}
	// warp collider (centered)
	//target.x = mod(target.x + _colliderSize.x / 2, gridSize.x * tileSize.x) - _colliderSize.x / 2;
	target.x = mod(target.x, gridSize.x * tileSize.x);
	//std::cout << std::endl;
	
	// 2. try to move vertically
	target.y += offset.y;
	// do collision resolution for every neighbour (dependend on collider size)
	for (auto i = 0; i <= std::ceil(_colliderSize.x / tileSize.x); ++i)
	{
		for (auto j = 0; j <= std::ceil(_colliderSize.y / tileSize.y); ++j)
		{
			// determine current tile position
			sf::Vector2i tilePos(i, j);
			tilePos.x += mod(target.x / tileSize.x, gridSize.x);		// asume integer division
			tilePos.y += mod(target.y / tileSize.y, gridSize.y);		// asume integer division
			// resolve collision if there is one
			if (map->isSolid(tilePos))
			{
				// if target is top compared to the current tile
				if (target.y < tilePos.y * tileSize.y)
				{
					target.y = tilePos.y * tileSize.y - _colliderSize.y;
				}
				// if target is bottom compared to the current tile
				else
				{
					target.y = tilePos.y * tileSize.y + tileSize.y;
				}
			}
		}
	}
	// warp collider (centered)
	//target.y = mod(target.y + _colliderSize.y / 2, gridSize.y * tileSize.y) - _colliderSize.y / 2;
	target.y = mod(target.y, gridSize.y * tileSize.y);
	
	// TODO add oneself on the new position to the grid data
	
	// move the shape
	_colliderPos = target;
	_shape.setPosition(target.x, target.y);
	
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
