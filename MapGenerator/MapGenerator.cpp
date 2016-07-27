#include "MapGenerator.h"


MapGenerator::MapGenerator()
{
	int tiles_count;
	std::ifstream file;
	file.open(MapGenerator::Path + MapGenerator::TilesetName + "." + MapGenerator::Extension, std::ifstream::in);
	file >> tiles_count;
	for (unsigned int tile_id = 0; tile_id < tiles_count; tile_id++)
	{
		for (unsigned int rotation = 0; rotation < 4; rotation++)
		{
			Tile tile(tile_id, rotation);
			tileset.push_back(tile);
		}
	}
	file.close();
}

/**********************************************************************************************************************/


MapGenerator::~MapGenerator()
{
}