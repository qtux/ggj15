/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

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
