#pragma once

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	Tile();
	
	bool walkable;
	
	virtual bool intersects(const GameObject& cmp) const;
	virtual bool intersects(const sf::Vector2f &testPos, const GameObject& cmp) const;

	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	
};
