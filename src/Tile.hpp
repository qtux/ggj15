#pragma once

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	Tile();
	
	bool walkable;
	
	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	
};
