#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>

#include "..\Common\Tile.h"

/******************************************************************************/


class MapGenerator
{
/******************************************************************************/

private :
	const std::string Path = "Resources\\";
	const std::string TilesetName = "Rooms";
	const std::string Extension = "txt";

	enum Way
	{
		Absent = 0,
		Exist = 1,
		Possible = 2
	};

	const int EmptyTileId = 0;
	const int NoTileIndx = -1;

	std::vector<Tile> tileset;
	sf::Vector2i size;
	std::vector<std::vector<int>> tiles;
/******************************************************************************/

public:
	MapGenerator();
	void createMap(sf::Vector2i size);
	void generate();
	void dbg_Print();
	void saveMap(std::string filename);
	~MapGenerator();

private:
/******************************************************************************/
	sf::Vector2i genFirstTile();
	void genNeighborTiles(sf::Vector2i position);
	void genTile(sf::Vector2i position);
	int findTile(int up, int right, int down, int left);
};

