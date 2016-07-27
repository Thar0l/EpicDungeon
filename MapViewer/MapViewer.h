#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <SFML\Graphics.hpp>

#include "..\Common\Tile.h"

class MapViewer
{
private:
	std::string filename;
	sf::RenderWindow window;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Vector2i size;

public:
	MapViewer();
	~MapViewer();
};

