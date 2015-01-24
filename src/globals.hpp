#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>
#include <string>
#include "SceneManager.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"

// use absolute paths on windows (strange behaviour)
#ifndef _WIN32
	#define PATH "./"
#else
	#define PATH "C:/GalacticIrrweg/trunk/VSPROJ/Project1/Debug/"
#endif

// window properties
static const int screenWidth = 800;
static const int screenHeight = 600;

// define number of keyboards binding 
#define INPUT_SIZE 15
typedef std::bitset<INPUT_SIZE> InputType;
extern sf::Keyboard::Key keyboardBinding[INPUT_SIZE];

extern sf::RenderWindow window;
extern SceneManager sceneManager;
extern SoundManager soundManager;
extern TextureManager textureManager;

extern bool focus;
extern InputType input;
extern sf::Clock globalClock;
