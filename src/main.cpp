/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "global.hpp"
#include "SceneManager.hpp"
#include "Menu.hpp"

int main()
{
	// define a window to draw scenes
	sf::ContextSettings settings(0, 0, 8);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Irrweg-Pizza", sf::Style::Default, settings);
	SceneManager sceneManager(new Menu(), window);
	// game loop
	while (window.isOpen())
	{
		sceneManager.update();
		sceneManager.redraw();
	}
}
