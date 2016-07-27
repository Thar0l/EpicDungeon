#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Tile.h"

/******************************************************************************/


class MapGenerator
{
/******************************************************************************/

private :
	sf::Vector2i size;
	std::vector<Tile> tileset;
/******************************************************************************/

public:
	MapGenerator();
};

