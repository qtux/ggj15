#pragma once

#include "GameObject.hpp"
#include "global.hpp"
#include "Level.hpp"
#include <queue>

class Player
{
public:
	Player(Level* level, const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize);
	float animationStep;
	int direction;
	float doggieStep;
	
	virtual bool intersects(const GameObject& cmp);
	virtual bool intersects(const sf::Vector2f &testPos, const GameObject& cmp);
	
	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	
	sf::Sprite * doggieSprite;
	sf::Sprite * mySprite;
private:
	std::queue<sf::Vector2f> positionQueue;
	std::queue<int> directionQueue;
	Level* level;
};

const static int PlayerAnimState[] = {0, 1, 1, 0, 2, 2};
const static int DoggieAnimState[] = {0, 2, 1, 3, 1, 4};
