#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>
#include <string>
#include "SceneManager.hpp"
#include "SoundManager.hpp"

// window properties
static const int screenWidth = 800;
static const int screenHeight = 600;

// define number of keyboards binding 
#define INPUT_SIZE 5
typedef std::bitset<INPUT_SIZE> InputType;
extern sf::Keyboard::Key keyboardBinding[INPUT_SIZE];

extern sf::RenderWindow window;
extern SceneManager sceneManager;
extern SoundManager soundManager;

extern bool focus;
extern InputType input;
extern sf::Clock globalClock;

// use absolute paths on windows and disable audio (strange behaviour)
#ifndef OS_WINDOWS
	static const std::string path =  "./";
	static const bool enableAudio = true;
#else
	static const std::string path =  "C:/GalacticIrrweg/trunk/VSPROJ/Project1/Debug/";
	static const bool enableAudio = false;
#endif
