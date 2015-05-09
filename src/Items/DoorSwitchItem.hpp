#ifndef DOORSWITCHITEM_H_
#define DOORSWITCHITEM_H_

#include "../Item.hpp"

class Level;

class DoorSwitchItem: public Item
{
private:
	bool toggled;
	sf::IntRect doorSwitchLeftTexture;
	sf::IntRect doorSwitchRightTexture;
public:
	DoorSwitchItem(sf::Sprite* sprite, bool toggled = false);
	virtual ~DoorSwitchItem();

	virtual bool applyEffect(Level& level);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual void update(sf::Time deltaTime);
};

#endif /* DOORSWITCHITEM_H_ */
