/*
 * NPC.cpp
 *
 *  Created on: 25.02.2015
 *      Author: johannes
 */

#include "NPC.hpp"
#include "globals.hpp"
#include <algorithm>
#include "Tile.hpp"
#include <math.h>
#include <iostream>

float mag2(const sf::Vector2f & a)
{
	return a.x * a.x + a.y * a.y;
}


NPC::NPC() : animationStep(0.), phase(drand48()) {
	// TODO Auto-generated constructor stub

}

NPC::~NPC() {
	// TODO Auto-generated destructor stub
}


bool NPC::intersects(const GameObject& cmp) const
{
	const sf::FloatRect &tmpRect = mySprite->getGlobalBounds();
	sf::Vector2f tmpPos(tmpRect.left, tmpRect.top);
	return intersects(tmpPos, cmp);
}

bool NPC::intersects(const sf::Vector2f &testPos, const GameObject& cmp) const
{ //TODO: könnte man mit der Struktur in Player besser verbinden
	if (dynamic_cast<const Tile*>(&cmp) && dynamic_cast<const Tile*>(&cmp)->walkable) return false; // TODO: aus intersect in allgemeineren Teil verschieben
	sf::FloatRect tmpRect(testPos.x + 3 * mySprite->getScale().x, testPos.y + 3 * mySprite->getScale().y, 10* mySprite->getScale().x, 10* mySprite->getScale().y);
	/*
	tmpRect.top += 32 - 10;
	tmpRect.left += 3;
	tmpRect.width = 10;
	tmpRect.height = 10;*/

	if (cmp.mySprite == 0) return false;
	/*std::cout<<"size test "<<tmpRect.left<<", "<<tmpRect.top<<", "<<tmpRect.width<<", "<<tmpRect.height<<", "<<std::endl;
	const sf::FloatRect &tmpRect2 = cmp.mySprite->getGlobalBounds();
	std::cout<<"size tile "<<tmpRect2.left<<", "<<tmpRect2.top<<", "<<tmpRect2.width<<", "<<tmpRect2.height<<", "<<std::endl;*/
	return cmp.mySprite->getGlobalBounds().intersects(tmpRect);
}


void NPC::update(sf::Time deltaTime) {
	float dT = deltaTime.asSeconds();
	//findPath dann ausführen wenn sich die Player-Position um bestimmten Werte geändert hat (wg. laufen oder Teleport)

	animationStep += 120 * dT / NPCslowFactor;
	if (animationStep >= 4.) animationStep -= 4;

	GameObject * myPlayer = gb::sceneManager.getCurrentScene().player;
	if (myPlayer == 0) return;

	if (path.size() == 0 || mag2(myPlayer->getPosition() - oldPlayerPos) > 9 * gb::pixelSizeX * gb::pixelSizeX)
	{
		path.clear();
		findPath(*this, *myPlayer, path);
		//std::cout<<"len path "<<path.size()<<std::endl;
		pathIdx = path.size() - 1;
		oldPlayerPos = myPlayer->getPosition();
	}

	if (pathIdx < 0) return;

	sf::Vector2f myPos = getPosition();

	if (mag2(path[pathIdx] - myPos) < 3) // 3 ist beliebiger aber fester Abstandsparameter vom KI-Wegpunkt
	{
		--pathIdx;
	}

	if (pathIdx < 0) return; // nicht bewegen wenn am Ende des Pfades

	sf::Vector2f moveVec = path[pathIdx] - myPos;
	moveVec /= sqrtf(mag2(moveVec));


	sf::Vector2f oldPos(myPos);

	myPos += moveVec * (120 * dT);

	phase += dT;
	if (phase > 2*M_PI) phase -= 2*M_PI;

	myPos.y += sin(phase);

	int chkColl[] = {0, 0};
	checkTilesCollision(myPos, oldPos, chkColl);



	setPosition(myPos);
	mySprite->setTextureRect(sf::IntRect(0, NPCAnimState[int(animationStep)] * gb::pixelSizeY, gb::pixelSizeX, gb::pixelSizeY));
}

void NPC::expandNode(const std::vector<GameObject*> &myBoard, npc::step &current, std::vector<npc::step> &openList, std::vector<npc::step> &closedList, const sf::Vector2f &destPos)
{
	float currCost = current.cost + 1.f;

	for (auto &tmp : myBoard)
	{
		if (!dynamic_cast<const Tile*>(tmp)->walkable) continue;

		const sf::Vector2f &tmpPos = tmp->getPosition();// + sf::Vector2f(gb::pixelSizeX/2,  gb::pixelSizeY/2);
		sf::Vector2f dist = current.pos - tmpPos;
		if (mag2(dist) < .5*(gb::pixelSizeX * gb::pixelSizeX + gb::pixelSizeY * gb::pixelSizeY) + 10) //10 just to be safe
		{
			// is current check position already in closed list -> ignore
			bool doContinue = false;
			for (auto &chk : closedList)
			{
				if (chk.pos == tmpPos)
				{
					doContinue = true;
				}
			}
			if (doContinue) continue;

			// is current position in open list but cheaper to reach -> ignore, otherwise replace predecessor with current position
			for (auto &chk : openList)
			{
				if (tmpPos == chk.pos)
				{
					if (chk.cost > currCost)
					{
						chk.cost = currCost;
						sf::Vector2f tmpDist = tmpPos - destPos;
						chk.value = mag2(tmpDist) + 0.* gb::pixelSizeX * drand48() * 2;
						chk.from = current.pos;
					}
					doContinue = true;
				}
			}
			if (doContinue) continue;

			sf::Vector2f tmpDist = tmpPos - destPos;
			openList.push_back(npc::step(tmpPos, current.pos, currCost, mag2(tmpDist) + 0. * gb::pixelSizeX * drand48() * 2));
			std::push_heap(openList.begin(), openList.end(), npc::step::comp);
		}
	}
}

void NPC::findPath(const GameObject& from, const GameObject& to,
		std::vector<sf::Vector2f>& path) {
	const std::vector<GameObject*> &myBoard = gb::sceneManager.getCurrentScene().gameBoard; //TODO: define type (class or typedef) for board? (johannes)

	std::vector<npc::step> openList; //http://www.cplusplus.com/reference/algorithm/make_heap/
	std::make_heap(openList.begin(), openList.end(), npc::step::comp);
	std::vector<npc::step> closedList;
	sf::Vector2f destPos = to.getPosition() + sf::Vector2f(10,24); //TODO: besser lösen, aktuell Player-Pixelbreite etc. hard-coded!
	sf::Vector2f origPos = from.getPosition();

//	std::cout<<"from: "<<origPos.x<<", "<<origPos.y<<std::endl;
//	std::cout<<"to: "<<destPos.x<<", "<<destPos.y<<std::endl;

	openList.push_back(npc::step(origPos, origPos, 0, 0));
	std::push_heap(openList.begin(), openList.end(), npc::step::comp);

	bool reached = false;
	while (!reached && openList.size() > 0)
	{
		closedList.push_back(openList.front());
		npc::step &current = closedList.back();
		if (mag2(current.pos - destPos) < gb::pixelSizeX * gb::pixelSizeX + gb::pixelSizeY * gb::pixelSizeY + 10)
		{
			reached = true;
			continue;
		}

		std::pop_heap(openList.begin(), openList.end(), npc::step::comp);
		openList.pop_back();

		expandNode(myBoard, current, openList, closedList, destPos);
	}

	if (reached)
	{
		//std::cout<<"reached"<<std::endl;
		path.push_back(destPos);
		npc::step *curr = &(closedList.back());
		bool back = false;
		while (curr->pos != origPos)
		{
			path.push_back(curr->pos);
			for (auto &wb : closedList)
			{
				if (wb.pos == curr->from) curr = &wb;
			}
		}
		path.push_back(curr->pos);
	}

}

void NPC::draw(sf::RenderTarget& renderTarget, sf::Shader* renderShader) {
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);

		for (auto &it : path)
		{
			sf::CircleShape shape(5);

			// set the shape color to green
			shape.setFillColor(sf::Color(250, 50, 50));
			shape.setPosition(it);
			renderTarget.draw(shape);
		}
	}
}
