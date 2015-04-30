#include "global.hpp"
#include "SceneManager.hpp"
#include "Menu.hpp"

int main() {
	
	// define a window to draw scenes
	//sf::ContextSettings settings(0, 0, gb::antialiasingLevel);
	//sf::RenderWindow window(sf::VideoMode(gb::screenWidth, gb::screenHeight), "Scene Test", sf::Style::Default, settings);
	SceneManager sceneManager(new Menu(), gb::window);
	// game loop
	while (gb::window.isOpen())
	{
		sceneManager.update();
		sceneManager.redraw();
	}
}
