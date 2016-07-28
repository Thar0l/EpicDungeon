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

	struct Room
	{
		int tile_id;
		int zone_id;
	};

	const int EmptyTileId =  0;
	const int NoTileIndex  = -1;
	const int EmptyZoneId =  0;
	const int NoZoneIndex = -1;

	std::vector<Tile> tileset;
	sf::Vector2i size;
	std::vector<std::vector<Room>> tiles;
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
	int findZones();
	void clearZones();
	bool addToZone(sf::Vector2i position, int zone_id);
	sf::Vector2i findZonesBorder();
	void deleteZonesBorder(sf::Vector2i position);
};

