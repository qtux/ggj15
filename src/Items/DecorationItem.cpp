#include "../global.hpp"
#include "DecorationItem.hpp"
#include "../Level.hpp"

DecorationItem::DecorationItem(sf::Sprite* sprite, bool blocksPath_, int texPosX_, int texPosY_, int texW_, int texH_):
	Item(sprite, texPosX_, texPosY_, texW_, texH_)
{
	blocksPath = blocksPath_;
	collectable = false;
}

bool DecorationItem::applyEffect(Level& level)
{
	return false;
}

void DecorationItem::update (sf::Time deltaTime) {}

void DecorationItem::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != nullptr)
	{
		renderTarget.draw(*mySprite);
	}
}
