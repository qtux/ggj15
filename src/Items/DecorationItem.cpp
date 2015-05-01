#include "../global.hpp"
#include "DecorationItem.hpp"
#include "../Level.hpp"

DecorationItem::DecorationItem(sf::Sprite* sprite, bool blocksPath_, int texPosX_, int texPosY_, int texW_, int texH_):Item(sprite,texPosX_,texPosY_,texW_,texH_) {
	this->blocksPath = blocksPath_;
	this->collectable = false;
}

bool DecorationItem::applyEffect(Level& level)
{
	return false;
};


void DecorationItem::update (sf::Time deltaTime) {
	if (mySprite != nullptr)
	{
		mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
		//sf::Vector2f vec = getPosition();
		//setPosition(vec.x,vec.y+(std::sin((waveClock.getElapsedTime().asSeconds() * 1000 +offset)/300)+0)*0.05);
		
	}
};

void DecorationItem::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != nullptr)
	{
		renderTarget.draw(*mySprite);
	}
}
