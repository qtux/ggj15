#pragma once
#include <string>

class TextElement {
public:
	TextElement();
	std::string eventType;
	bool bold;
	int r;
	int g;
	int b;
	std::string text;
};
