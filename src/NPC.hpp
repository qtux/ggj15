#pragma once


#include "GameObject.hpp"


float mag2(const sf::Vector2f & a);

namespace npc{
struct step {
	sf::Vector2f pos;
	sf::Vector2f from;
	float cost;
	float value;

	step(const sf::Vector2f &pos_, const sf::Vector2f &from_, float cost_, float value_): pos(pos_), from(from_), cost(cost_), value(value_) {}

	inline friend bool operator<(const step& a, const step& b)
	{
		//std::cout<<"werde aufgerufen!"<<std::endl;
		return (a.value > b.value);
	}

};

/*bool comp(const step &a, const step &b)
{
	return (a.value < b.value);
}*/
// adjusted from http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
/*class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (const step& lhs, const step&rhs) const
  {
    if (reverse) return (lhs.value>rhs.value);
    else return (lhs.value<rhs.value);
  }
};*/


//typedef std::priority_queue<step,std::vector<step>,mycomparison> mypq_type;

}



class NPC: public GameObject {
private:
	int direction;
	float animationStep;
	sf::Vector2f oldPlayerPos;
	int pathIdx;
	std::vector<sf::Vector2f> path;
	float phase;

	void expandNode(const std::vector<GameObject*> &myBoard, npc::step &current, std::vector<npc::step> &openList, std::vector<npc::step> &closedList, const sf::Vector2f &destPos);
	void findPath(const GameObject &from, const GameObject &to, std::vector<sf::Vector2f> &path);

public:
	NPC();
	virtual ~NPC();

	virtual bool intersects(const GameObject& cmp) const;
	virtual bool intersects(const sf::Vector2f &testPos, const GameObject& cmp) const;

	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
};

const static int NPCAnimState[] = { 1, 0, 2, 0 };

const static int NPCslowFactor = 15;
