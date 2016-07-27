#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Tile.h"

/******************************************************************************/


class MapGenerator
{
/******************************************************************************/

private :
	const std::string Path = "Resources\\";
	const std::string TilesetName = "Rooms";
	const std::string Extension = "txt";
	sf::Vector2i size;
	std::vector<Tile> tileset;
/******************************************************************************/

public:
	MapGenerator();
	~MapGenerator();
};

