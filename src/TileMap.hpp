/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <cmath>
#include <vector>
#include <set>
#include <forward_list>
#include <SFML/Graphics.hpp>
class Entity;

class TileMap
{
public:
	sf::Vector2u tileSize;			// pixels x pixels
	sf::Vector2u gridSize;			// tiles x tiles
	TileMap(const sf::Vector2u& tileSize, const sf::Vector2u& gridSize, const std::string fileName);
	void switchTile(const sf::Vector2u& first, const sf::Vector2u& second, const sf::Time& duration);
	void switchRange(const sf::Vector2u& first, const sf::Vector2u& second, const sf::Vector2u& size, const sf::Time& duration);
	void update(const sf::Time& deltaT);
	void draw(sf::RenderTarget& target);
	bool isSolid(sf::Vector2i pos) const
	{
		return _solid[pos.x + pos.y * gridSize.x];
	}
	void add(Entity* entity, const sf::Vector2i& pos)
	{
		_entities[pos.x + pos.y * gridSize.x].insert(entity);
	}
	void remove(Entity* entity, const sf::Vector2i& pos)
	{
		_entities[pos.x + pos.y * gridSize.x].erase(entity);
	}
	void removeAll(Entity* entity)
	{
		for (auto& tile: _entities)
		{
			tile.erase(entity);
		}
	}
	const std::set<Entity*>& getEntities(const sf::Vector2i& pos) const
	{
		return _entities[pos.x + pos.y * gridSize.x];
	}
private:
	struct TileAnimation
	{
		TileAnimation(sf::Sprite* sprite, const sf::Vector2f& startPos, const sf::Vector2f& targetPos, const sf::Time& duration, const sf::Vector2u& quadIndex):
			sprite(sprite),
			quadIndex(quadIndex),
			_startPos(startPos),
			_targetPos(targetPos),
			_duration(duration),
			_t(0.0f)
		{
			// define a random intial velocity dependend on positions and duration
			_velocity.x = 2.0f * rand() / RAND_MAX - 1.0f;
			_velocity.y = 2.0f * rand() / RAND_MAX - 1.0f;
			_velocity /= getLength(_velocity);
			_velocity *= getLength((_targetPos - _startPos) / _duration.asSeconds());
			sprite->setPosition(_startPos);
		}
		
		~TileAnimation()
		{
			delete sprite;
		}
		
		bool update(const sf::Time& deltaT)
		{
			// determine metrics
			_t += deltaT.asSeconds() / _duration.asSeconds();
			float remaining = getLength(_targetPos - sprite->getPosition());
			float traveled = getLength(sprite->getPosition() - _startPos);
			float total = remaining + traveled;
			
			// stop the animation if the target is reached
			if (remaining / total < 0.001f || _t > 1)
			{
				sprite->setPosition(_targetPos);
			}
			else
			{
				// LERP of the previous velocity and the velocity required to reach the target in 0.4f * duration
				_velocity = (1 - _t) * _velocity + _t * (_targetPos - sprite->getPosition()) / (0.4f * _duration.asSeconds());
				sprite->move(deltaT.asSeconds() * _velocity);
				// determine the scaling factor based on traveled and remaining distance
				float scale = (std::min(traveled, remaining) + total) / total;
				sprite->setScale(scale, scale);
			}
			// return true if the animation is finished
			return _t > 1;
		}
		
		sf::Sprite* sprite;
		const sf::Vector2u quadIndex;
	private:
		float getLength(const sf::Vector2f& vec)
		{
			return std::hypot(vec.x, vec.y);
		}
		const sf::Vector2f _startPos;
		const sf::Vector2f _targetPos;
		const sf::Time _duration;
		sf::Vector2f _velocity;
		float _t;
	};
	sf::VertexArray _vertices;
	const sf::Texture* _baseTileSet, *_texture;
	std::vector<unsigned int> _mapping;
	std::vector<bool> _solid;
	std::vector<std::set<Entity*>> _entities;
	std::forward_list<TileAnimation> _movingTiles;
};
