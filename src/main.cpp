#include "global.hpp"
#include "SceneManager.hpp"
#include "Menu.hpp"

int main()
{
	// define a window to draw scenes
	sf::ContextSettings settings(0, 0, 8);
	sf::RenderWindow window(sf::VideoMode(gb::screenWidth, gb::screenHeight), "Irrweg-Pizza", sf::Style::Default, settings);
	SceneManager sceneManager(new Menu(), window);
	// game loop
	while (window.isOpen())
	{
		sceneManager.update();
		sceneManager.redraw();
	}
}
