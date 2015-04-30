#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include "global.hpp"

SceneManager::SceneManager(Scene* initialScene, sf::RenderWindow& window):
	_nextScene(initialScene),
	_currentScene(initialScene),
	_focus(true),
	_window(window)
{
	_currentScene->resize(_window.getSize().x, _window.getSize().y, _window);
}

void SceneManager::update()
{
	// retrieve input (either gamepad or keyboard)
	if (sf::Joystick::isConnected(0)) {
		// retrieve current gamepad gb::input
		gb::input[0] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == -100;
		gb::input[1] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == 100;
		gb::input[2] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == -100;
		gb::input[3] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == 100;
		// TODO input 4 - 7 buggy?
		for (int i = 0; i < 3; ++i) {
			gb::input[i + 4] = sf::Joystick::isButtonPressed(0, i);
		}
	}
	else {
		// retrieve current keyboard input
		for (int i = 0; i < INPUT_SIZE; ++i) {
			gb::input[i] = sf::Keyboard::isKeyPressed(gb::keyboardBinding[i]) && _focus;
		}
	}
	
	// process events
	sf::Event event;
	while (_window.pollEvent(event))
	{
		// common event processing
		switch (event.type)
		{
			case sf::Event::Closed:
				_window.close();
				break;
			case sf::Event::LostFocus:
				_focus = false;
				break;
			case sf::Event::GainedFocus:
				_focus = true;
				break;
			case sf::Event::Resized:
				_currentScene->resize(event.size.width, event.size.height, _window);
				break;
			default:
				break;
		}
		// scene dependent event processing (call once per event)
		if (_focus)
		{
			_nextScene = _currentScene->processEvent(event, _window);
			// immediately set currentScene to _nextScene if it is different from the _nextScene
			if (_currentScene != _nextScene)
			{
				delete _currentScene;
				_currentScene = _nextScene;
				_currentScene->resize(_window.getSize().x, _window.getSize().y, _window);
			}
		}
	}
	
	// reset clock and determine elapsed time since last frame
	sf::Time deltaT = _clock.restart();
	
	// call update once on current scene
	if (_focus)
	{	
		_currentScene->update(deltaT, _window);
	}
}

void SceneManager::redraw()
{
	_window.clear();
	_currentScene->draw(_window, _focus);
	_window.display();
}
