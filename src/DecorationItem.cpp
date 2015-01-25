#include "globals.hpp"
#include "DecorationItem.hpp"

DecorationItem::DecorationItem(sf::Sprite* sprite, bool blocksPath_, int texPosX_, int texPosY_, int texW_, int texH_):Item(sprite,texPosX_,texPosY_,texW_,texH_) {
	this->blocksPath = blocksPath_;
	this->collectable = false;
}

void DecorationItem::applyEffect()
{
};


void DecorationItem::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
		//sf::Vector2f vec = getPosition();
		//setPosition(vec.x,vec.y+(std::sin((waveClock.getElapsedTime().asMilliseconds()+offset)/300)+0)*0.05);
		window.draw(*mySprite);
	}
};
