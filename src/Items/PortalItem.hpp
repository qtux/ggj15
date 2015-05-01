#pragma once

#include "../Item.hpp"

class Level;

class PortalItem : public Item
{
public:
	PortalItem(sf::Sprite* sprite);
	virtual ~PortalItem();
	bool applyEffect(Level& level);
	virtual void update(sf::Time deltaTime);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
private:
	sf::Clock animClock;
	bool neverAgain;
};
