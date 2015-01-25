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
static const int pixelSizeX = 16;
static const int pixelSizeY = 16;
static const int sizeX = 5;
static const int sizeY = 4;
static const int largeTileSizeX = 6;
static const int largeTileSizeY = 6;
static const int gridWidth = largeTileSizeX * sizeX * pixelSizeX;
static const int gridHeight = largeTileSizeY * sizeY * pixelSizeY;
static const int screenWidth = 800;
static const int screenHeight = 600;

// define number of keyboards binding 
#define INPUT_SIZE 6
typedef std::bitset<INPUT_SIZE> InputType;
extern sf::Keyboard::Key keyboardBinding[INPUT_SIZE];

extern sf::RenderWindow window;
extern SceneManager sceneManager;
extern SoundManager soundManager;
extern TextureManager textureManager;

extern bool focus;
extern InputType input;
extern sf::Clock globalClock;
extern sf::RectangleShape background;
