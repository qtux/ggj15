#include "Player.hpp"
#include "global.hpp"
#include <cassert>

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
	_colliderPos.y += 32;
	_colliderSize.y -=32;
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
// TODO remove oneself from the grid data and add again
void Player::move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map)
{
	// define lambdas for cleaner code
	auto mod = [] (int a, int b) {return (a < 0) ? a % b + b : a % b;};
	auto ceilDiv = [] (int a, int b) {return (a % b) ? a / b + 1 : a / b;};
	auto floorDiv = [] (int a, int b) {return (a - ((a < 0) ? b - 1 : 0)) / b;};
	auto signum = [] (int a) {return (0 < a) - (a < 0);};
	// determine grid and tile size TODO move elsewhere?
	sf::Vector2i gridSize(gb::sizeX * gb::largeTileSizeX, gb::sizeY * gb::largeTileSizeY);
	sf::Vector2i tileSize(gb::pixelSizeX, gb::pixelSizeY);
	
	// TODO normalize moveDir?
	sf::Vector2i offset;
	offset.x = std::round(moveDir.x * velocity * deltaTime.asSeconds());
	offset.y = std::round(moveDir.y * velocity * deltaTime.asSeconds());
	assert(offset.x < tileSize.x && offset.y < tileSize.y);
	
	// first move the collider horizontally (avoid jittery movement by rounding) and resolve
	_colliderPos.x += offset.x;
	int penetration = 0;
	// do collision resolution for every neighbour (dependend on collider size)
	for (int i = 0; i <= ceilDiv(_colliderSize.x, tileSize.x); ++i)
	{
		for (int j = 0; j <= ceilDiv(_colliderSize.y, tileSize.y); ++j)
		{
			// determine rects to be tested for intersection
			sf::IntRect tile((i + floorDiv(_colliderPos.x, tileSize.x)) * tileSize.x, (j + floorDiv(_colliderPos.y, tileSize.y)) * tileSize.y, tileSize.x, tileSize.y);
			sf::IntRect collider(_colliderPos, _colliderSize);
			sf::IntRect intersection;
			// determine current warped tile coordinates
			sf::Vector2i tileCoord;
			tileCoord.x = mod(i + floorDiv(_colliderPos.x, tileSize.x), gridSize.x);
			tileCoord.y = mod(j + floorDiv(_colliderPos.y, tileSize.y), gridSize.y);
			// detect collision if there is one and the collider intersects with the tile
			if (map->isSolid(tileCoord) && collider.intersects(tile, intersection))
			{
				penetration = intersection.width * signum(tile.left - collider.left);
			}
			//std::cout << "collider: " << collider.left << "|" << collider.top << " tile: " << tile.left << "|" << tile.top << " (solid: " << map->isSolid(tileCoord) << " intersection: " << collider.intersects(tile) << " and: " << (collider.intersects(tile) && map->isSolid(tileCoord)) << ")" << std::endl;
		}
	}
	// resolve a vertical collision subtracting the penetration value
	_colliderPos.x -= penetration;
	// and the warp the collider relating to its center
	_colliderPos.x = mod(_colliderPos.x + _colliderSize.x / 2, gridSize.x * tileSize.x) - _colliderSize.x / 2;
	
	// then move the collider vertically (avoid jittery movement by rounding) and resolve
	_colliderPos.y += offset.y;
	penetration = 0;
	// do collision resolution for every neighbour (dependend on collider size)
	for (int i = 0; i <= ceilDiv(_colliderSize.x, tileSize.x); ++i)
	{
		for (int j = 0; j <= ceilDiv(_colliderSize.y, tileSize.y); ++j)
		{
			// determine rects to be tested for intersection
			sf::IntRect tile((i + floorDiv(_colliderPos.x, tileSize.x)) * tileSize.x, (j + floorDiv(_colliderPos.y, tileSize.y)) * tileSize.y, tileSize.x, tileSize.y);
			sf::IntRect collider(_colliderPos, _colliderSize);
			sf::IntRect intersection;
			// determine current warped tile coordinates
			sf::Vector2i tileCoord;
			tileCoord.x = mod(i + floorDiv(_colliderPos.x, tileSize.x), gridSize.x);
			tileCoord.y = mod(j + floorDiv(_colliderPos.y, tileSize.y), gridSize.y);
			// detect collision if there is one and the collider intersects with the tile
			if (map->isSolid(tileCoord) && collider.intersects(tile, intersection))
			{
				penetration = intersection.height * signum(tile.top - collider.top);
			}
			//std::cout << "collider: " << collider.left << "|" << collider.top << " tile: " << tileCoord.x << "|" << tileCoord.y << " (solid: " << map->isSolid(tileCoord) << " intersection: " << collider.intersects(tile) << " and: " << (collider.intersects(tile) && map->isSolid(tileCoord)) << ")" << std::endl;
		}
	}
	// resolve a vertical collision subtracting the penetration value
	_colliderPos.y -= penetration;
	// and the warp the collider relating to its center
	_colliderPos.y = mod(_colliderPos.y + _colliderSize.y / 2, gridSize.y * tileSize.y) - _colliderSize.y / 2;
	
	
	
	// move the shape
	_shape.setPosition(_colliderPos.x, _colliderPos.y - 32);
	
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
