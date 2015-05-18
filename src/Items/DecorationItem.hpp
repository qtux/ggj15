#pragma once

#include "../Item.hpp"

class Level;

class DecorationItem : public Item
{
public:
	DecorationItem(sf::Sprite* sprite, bool blocksPath, int texPosX, int texPosY, int texW, int texH);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	void update(sf::Time deltaTime);
};
