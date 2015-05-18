#pragma once

#include <SFML/Graphics.hpp>
class Level;

class Item
{
public:
	Item(sf::Sprite* sprite,int x,int y,int w,int h);
	Item(sf::Sprite* sprite);
	virtual ~Item();

	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual bool applyEffect(Level& level);
	
	const sf::Vector2f& getPosition() const;
	void setPosition(float x, float y);
	
	sf::Sprite * mySprite;
	bool collectable;
	bool blocksPath;
	int drawX;
	int drawY;
	int drawW;
	int drawH;
private:
	float offset;
	sf::Clock waveClock;
};
