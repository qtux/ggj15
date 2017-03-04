#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Menu.hpp"

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
	
	// call update once on current scene
	if (_focus)
	{
		// determine elapsed time since last frame
		if (_clock.getElapsedTime() > sf::seconds(1.0f / 60.0f))
		{
			// reset clock if more than a 1/60 seconds have passed
			sf::Time deltaT = _clock.restart();
			_nextScene = _currentScene->update(deltaT, _window);
		}
		// immediately set currentScene to _nextScene if it is different from the _nextScene
		if (_currentScene != _nextScene)
		{
			delete _currentScene;
			_currentScene = _nextScene;
			_currentScene->resize(_window.getSize().x, _window.getSize().y, _window);
		}
	}
}

void SceneManager::redraw()
{
	_window.clear();
	_currentScene->draw(_window, _focus);
	_window.display();
}
