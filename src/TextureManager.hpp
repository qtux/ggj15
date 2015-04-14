#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class TextureManager
{
public:
	const sf::Texture& getTexture(std::string fileName, bool repeat, bool smooth = false);
	const sf::Texture& getTileSet(const sf::Texture& baseTileSet, const std::vector<unsigned int>& mapping, const sf::Vector2u& tileSize, const sf::Vector2u& gridSize, bool smooth = false);
private:
	struct Key
	{
		Key() = delete;
		Key(std::string fileName, bool repeat, bool smooth):
			fileName(fileName), repeat(repeat), smooth(smooth)
		{}
		std::string fileName;
		bool repeat;
		bool smooth;
		bool operator<(const Key& rhs) const
		{
			// compare fileName then repeat and finally smooth
			return std::tie(fileName, repeat, smooth) < std::tie(rhs.fileName, rhs.repeat, rhs.smooth);
		}
	};
	std::map<Key, sf::Texture> _textures;
	sf::RenderTexture renderTex;
};

// TODO maximum size of textures dependend on driver
// (usually in the range of 512 up to 8192 pixel per dimension)
// TODO use pointer instead of references to be able to release resources?
// maybe use shared pointer to automatically release resources?
