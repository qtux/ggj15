#pragma once

#include <SFML/Graphics.hpp>

class Scene;

class SceneManager
{
public:
	SceneManager(Scene* initialScene, sf::RenderWindow& window);
	void update();
	void redraw();
private:
	Scene* _nextScene;
	Scene* _currentScene;
	bool _focus;
	sf::Clock _clock;
	sf::RenderWindow& _window;
};

// TODO set up a rudimentary input binding
