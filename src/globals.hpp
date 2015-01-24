#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>
#include "SceneManager.hpp"
#include "SoundManager.hpp"

static const int screenWidth = 800;
static const int screenHeight = 600;


// define number of keyboards binding 
#define INPUT_SIZE 5
typedef std::bitset<INPUT_SIZE> InputType;
extern sf::Keyboard::Key keyboardBinding[INPUT_SIZE];

extern sf::RenderWindow window;
extern SceneManager sceneManager;
extern SoundManager soundManager;
extern SoundManager textureManager;

extern bool focus;
extern InputType input;
