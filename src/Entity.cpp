#include "Entity.hpp"
#include "TileMap.hpp"
#include "global.hpp"
#include <cassert>

Entity::Entity(float velocity, const sf::IntRect& collider):
	_velocity(velocity),
	_collider(collider)
{}

bool Entity::intersects(const sf::Vector2i& tilePosition, const sf::Vector2i& tileSize)
{
	sf::Vector2i tileCoord(tilePosition.x * tileSize.x, tilePosition.y * tileSize.y);
	sf::IntRect tileRect(tileCoord, tileSize);
	return _collider.intersects(tileRect);
}

bool Entity::intersects(const Entity& other, sf::IntRect& intersection)
{
	return _collider.intersects(other._collider, intersection);
}

bool Entity::intersects(const Entity& other)
{
	return _collider.intersects(other._collider);
}

// TODO use integer positions everywhere (do not use unsigned integer)
// TODO add a grid of collision/gameobject data (replace map)
// TODO add a method to move more than one tile wide
// TODO remove oneself from the grid data and add again
void Entity::atomicMove(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map)
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
	offset.x = std::round(moveDir.x * _velocity * deltaTime.asSeconds());
	offset.y = std::round(moveDir.y * _velocity * deltaTime.asSeconds());
	assert(offset.x < tileSize.x && offset.y < tileSize.y);
	
	// first move the collider horizontally (avoid jittery movement by rounding) and resolve
	_collider.left += offset.x;
	int penetration = 0;
	// do collision resolution for every neighbour (dependend on collider size)
	for (int i = 0; i <= ceilDiv(_collider.width, tileSize.x); ++i)
	{
		for (int j = 0; j <= ceilDiv(_collider.height, tileSize.y); ++j)
		{
			// determine rects to be tested for intersection
			sf::IntRect tile((i + floorDiv(_collider.left, tileSize.x)) * tileSize.x, (j + floorDiv(_collider.top, tileSize.y)) * tileSize.y, tileSize.x, tileSize.y);
			sf::IntRect intersection;
			// determine current warped tile coordinates
			sf::Vector2i tileCoord;
			tileCoord.x = mod(i + floorDiv(_collider.left, tileSize.x), gridSize.x);
			tileCoord.y = mod(j + floorDiv(_collider.top, tileSize.y), gridSize.y);
			// TODO get collision information at tileCoord
			// TODO define width
			/*auto& coll = colliderGrid[tileCoord.x + tileCoord.y * width]
			if (coll.first)
			{
				coll.second.erase(this);	// O(m * log(n)) if using a set, O(m * n) if using forward list
				// do collision check as before
			}
			else
			{
				for (auto& entity: coll.second)
				{
					if (entity == this)
					{
						// erase this --> O(m)
					}
					else
					{
						// check for collision --> collider of entites intersecting and do resolution
					}
				}
			}*/
			// TODO get new tileCoords and insert this (up to m times) --> O(m * log(n)) with set, O(m) with forward_list
			
			
			// detect collision if there is one and the collider intersects with the tile
			if (map->isSolid(tileCoord) && _collider.intersects(tile, intersection))
			{
				penetration = intersection.width * signum(tile.left - _collider.left);
			}
			//std::cout << "collider: " << _collider.left << "|" << _collider.top << " tile: " << tile.left << "|" << tile.top << " (solid: " << map->isSolid(tileCoord) << " intersection: " << _collider.intersects(tile) << " and: " << (_collider.intersects(tile) && map->isSolid(tileCoord)) << ")" << std::endl;
		}
	}
	// resolve a vertical collision subtracting the penetration value
	_collider.left -= penetration;
	// and the warp the collider relating to its center
	_collider.left = mod(_collider.left + _collider.width / 2, gridSize.x * tileSize.x) - _collider.width / 2;
	
	// then move the collider vertically (avoid jittery movement by rounding) and resolve
	_collider.top += offset.y;
	penetration = 0;
	// do collision resolution for every neighbour (dependend on collider size)
	for (int i = 0; i <= ceilDiv(_collider.width, tileSize.x); ++i)
	{
		for (int j = 0; j <= ceilDiv(_collider.height, tileSize.y); ++j)
		{
			// determine rects to be tested for intersection
			sf::IntRect tile((i + floorDiv(_collider.left, tileSize.x)) * tileSize.x, (j + floorDiv(_collider.top, tileSize.y)) * tileSize.y, tileSize.x, tileSize.y);
			sf::IntRect intersection;
			// determine current warped tile coordinates
			sf::Vector2i tileCoord;
			tileCoord.x = mod(i + floorDiv(_collider.left, tileSize.x), gridSize.x);
			tileCoord.y = mod(j + floorDiv(_collider.top, tileSize.y), gridSize.y);
			// detect collision if there is one and the collider intersects with the tile
			if (map->isSolid(tileCoord) && _collider.intersects(tile, intersection))
			{
				penetration = intersection.height * signum(tile.top - _collider.top);
			}
			//std::cout << "collider: " << _collider.left << "|" << _collider.top << " tile: " << tileCoord.x << "|" << tileCoord.y << " (solid: " << map->isSolid(tileCoord) << " intersection: " << _collider.intersects(tile) << " and: " << (_collider.intersects(tile) && map->isSolid(tileCoord)) << ")" << std::endl;
		}
	}
	// resolve a vertical collision subtracting the penetration value
	_collider.top -= penetration;
	// and the warp the collider relating to its center
	_collider.top = mod(_collider.top + _collider.height / 2, gridSize.y * tileSize.y) - _collider.height / 2;
}

void Entity::move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map)
{
	// TODO if offset < tileSize do atomicMove
	// TODO otherwise do multiple atomicMoves
	atomicMove(deltaTime, moveDir, sceneSize, map);
}
