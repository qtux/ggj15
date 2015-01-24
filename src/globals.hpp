#pragma once

// define number of keyboards binding 
#define INPUT_SIZE 5
typedef std::bitset<INPUT_SIZE> InputType;
static sf::Keyboard::Key keyboardBinding[INPUT_SIZE] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::A
};

static sf::RenderWindow window(sf::VideoMode(800, 600), "Galactic Irrweg");
static SceneManager sceneManager;
static SoundManager soundManager;

static bool focus = true;
static InputType input;
