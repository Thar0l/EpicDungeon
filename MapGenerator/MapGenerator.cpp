#include "MapGenerator.h"

//#define DEBUG_MODE

MapGenerator::MapGenerator()
{
	int tiles_count;
	std::ifstream file;
	file.open(MapGenerator::Path + MapGenerator::TilesetName + "." + MapGenerator::Extension, std::ifstream::in);
	if (file.fail())
	{
		std::cout << "No resources found!" << std::endl;
		system("pause");
		exit(-1);
	}
	file >> tiles_count;
	for (unsigned int tile_id = 0; tile_id < tiles_count; tile_id++)
	{
		if (tile_id == MapGenerator::EmptyTileId)
		{
			Tile tile(tile_id, 0);
			tileset.push_back(tile);
		}
		else
		{
			for (unsigned int rotation = 0; rotation < 4; rotation++)
			{
				Tile tile(tile_id, rotation);
				tileset.push_back(tile);
#ifdef DEBUG_MODE	
				std::cout << tile.getId() << " tile with rotation " << tile.getRotation() << " : " << std::endl;
				std::cout << "\t*" << tile.hasUpPath() << "*" << std::endl;
				std::cout << "\t" << tile.hasLeftPath() << "*" << tile.hasRightPath() << std::endl;
				std::cout << "\t*" << tile.hasDownPath() << "*" << std::endl << std::endl;
#endif	
			}
		}
	}
	file.close();
	size = sf::Vector2i(0, 0);
}

/**********************************************************************************************************************/


void MapGenerator::createMap(sf::Vector2i size)
{
	this->size = size;
	for (unsigned int x = 0; x < size.x; x++)
	{
		std::vector<MapGenerator::Room> line;
		for (unsigned int y = 0; y < size.y; y++)
		{
			MapGenerator::Room room;
			room.tile_id = MapGenerator::NoTileIndex;
			room.zone_id = MapGenerator::NoZoneIndex;
			line.push_back(room);
		}
		tiles.push_back(line);
	}
	for (unsigned int x = 0; x < size.x; x++)
	{
		tiles[x][0].tile_id = MapGenerator::EmptyTileId;
		tiles[x][size.y - 1].tile_id = MapGenerator::EmptyTileId;
	}
	for (unsigned int y = 0; y < size.y; y++)
	{	
		tiles[0][y].tile_id = MapGenerator::EmptyTileId;
		tiles[size.x - 1][y].tile_id = MapGenerator::EmptyTileId;
	}
}

/**********************************************************************************************************************/


void MapGenerator::generate()
{
	sf::Vector2i position = genFirstTile();
	genNeighborTiles(position);
	
	while (findZones() > 1)
	{
		sf::Vector2i border_tile = findZonesBorder();
		
		if (border_tile != sf::Vector2i(0, 0))
		{
			deleteZonesBorder(border_tile);
			clearZones();
		}
		
	}
	
}

/**********************************************************************************************************************/


void MapGenerator::dbg_Print()
{
	std::cout << std::endl << "dbg_map_tiles_ids:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			if (tiles[x][y].tile_id == MapGenerator::NoTileIndex)
			{
				std::cout << "-" << " ";
			}
			else
			{
				std::cout << tileset[tiles[x][y].tile_id].getId() << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "dbg_map_tiles_rotations:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			if (tiles[x][y].tile_id == MapGenerator::NoTileIndex)
			{
				std::cout << "-" << " ";
			}
			else
			{
				std::cout << tileset[tiles[x][y].tile_id].getRotation() << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "dbg_map_tiles_zones:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			if (tiles[x][y].zone_id == MapGenerator::NoZoneIndex)
			{
				std::cout << "-" << " ";
			}
			else
			{
				std::cout << tiles[x][y].zone_id << " ";
			}
		}
		std::cout << std::endl;
	}
}

/**********************************************************************************************************************/


void MapGenerator::saveMap(std::string filename)
{
	std::ofstream file;
	file.open(filename, std::ofstream::out);
	file << size.x << " " << size.y << std::endl;
	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			file << tileset[tiles[x][y].tile_id].getId() << " ";
		}
		file << std::endl;
	}
	file << std::endl;
	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			file << tileset[tiles[x][y].tile_id].getRotation() << " ";
		}
		file << std::endl;
	}
	file.close();
}

/**********************************************************************************************************************/


MapGenerator::~MapGenerator()
{
}

/**********************************************************************************************************************/


sf::Vector2i MapGenerator::genFirstTile()
{
	sf::Vector2i position = sf::Vector2i((rand() % (size.x - 2) + 1), (rand() % (size.y - 2) + 1));
	genTile(position);
#ifdef DEBUG_MODE
	std::cout << "Start tile position is (" << position.x << "; " << position.y << ")." << std::endl;
#endif
	return position;
}

/**********************************************************************************************************************/


void MapGenerator::genNeighborTiles(sf::Vector2i position)
{
	genTile(sf::Vector2i(position.x - 1, position.y));
	genTile(sf::Vector2i(position.x + 1, position.y));
	genTile(sf::Vector2i(position.x, position.y - 1));
	genTile(sf::Vector2i(position.x, position.y + 1));
}

/**********************************************************************************************************************/


void MapGenerator::genTile(sf::Vector2i position)
{
	if (tiles[position.x][position.y].tile_id == MapGenerator::NoTileIndex)
	{
		int up;
		int right;
		int down;
		int left;
		
		if (tiles[position.x][position.y-1].tile_id == MapGenerator::NoTileIndex)
		{
			up = MapGenerator::Way::Possible;
		} 
		else
		{
			up = tileset[tiles[position.x][position.y - 1].tile_id].getDownPath();
		}

		if (tiles[position.x + 1][position.y].tile_id == MapGenerator::NoTileIndex)
		{
			right = MapGenerator::Way::Possible;
		}
		else
		{
			right = tileset[tiles[position.x + 1][position.y].tile_id].getLeftPath();
		}

		if (tiles[position.x][position.y + 1].tile_id == MapGenerator::NoTileIndex)
		{
			down = MapGenerator::Way::Possible;
		}
		else
		{
			down = tileset[tiles[position.x][position.y + 1].tile_id].getUpPath();
		}

		if (tiles[position.x - 1][position.y].tile_id == MapGenerator::NoTileIndex)
		{
			left = MapGenerator::Way::Possible;
		}
		else
		{
			left = tileset[tiles[position.x - 1][position.y].tile_id].getRightPath();
		}
#ifdef DEBUG_MODE
		std::cout << "Finding tile for place (" << position.x << "; " << position.y << ")." << std::endl;
		std::cout << "Params is: " << up << " " << right << " " << down << " " << left << "." << std::endl;
#endif
		tiles[position.x][position.y].tile_id = findTile(up, right, down, left);
#ifdef DEBUG_MODE
		std::cout << "Found " << tileset[tiles[position.x][position.y]].getId() << " tile with rotation ";
		std::cout << tileset[tiles[position.x][position.y]].getRotation() << "." << std::endl;
#endif
		genNeighborTiles(position);
	}
	else
	{
#ifdef DEBUG_MODE
		std::cout << "Generating already existing tile (" << position.x << "; " <<position.y << ")!!!" << std::endl;
#endif
	}
}

/**********************************************************************************************************************/


int MapGenerator::findTile(int up, int right, int down, int left)
{
	
	std::vector<int> possibleTiles;
	for (unsigned int index = 0; index < tileset.size(); index++)
	{
		if ((up != MapGenerator::Way::Possible) && (tileset[index].getUpPath() != up))
		{
			continue;
		}
		if ((right != MapGenerator::Way::Possible) && (tileset[index].getRightPath() != right))
		{
			continue;
		}
		if ((down != MapGenerator::Way::Possible) && (tileset[index].getDownPath() != down))
		{
			continue;
		}
		if ((left != MapGenerator::Way::Possible) && (tileset[index].getLeftPath() != left))
		{
			continue;
		}
		possibleTiles.push_back(index);
	}
	if (possibleTiles.size() == 0)
	{
		std::cout << "Can't find tile!!!" << std::endl;
		return -2;
	}
	else
	{
		return possibleTiles[rand() % possibleTiles.size()];
	}
}

/**********************************************************************************************************************/


int MapGenerator::findZones()
{
	int zone_id = MapGenerator::EmptyZoneId + 1;
	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			if (tiles[x][y].tile_id == EmptyTileId)
			{
				tiles[x][y].zone_id = MapGenerator::EmptyZoneId;
			}
			else if (addToZone(sf::Vector2i(x, y), zone_id))
			{
				zone_id++;
			}
		}
	}
	return zone_id;
}

/**********************************************************************************************************************/

bool MapGenerator::addToZone(sf::Vector2i position, int zone_id)
{
	if (tiles[position.x][position.y].zone_id == MapGenerator::NoZoneIndex)
	{
		tiles[position.x][position.y].zone_id = zone_id;
		if ((position.x > 0) && (tileset[tiles[position.x][position.y].tile_id].hasLeftPath()))
		{
			addToZone(sf::Vector2i(position.x - 1, position.y), zone_id);
		}
		if ((position.x < (size.x - 1)) && (tileset[tiles[position.x][position.y].tile_id].hasRightPath()))
		{
			addToZone(sf::Vector2i(position.x + 1, position.y), zone_id);
		}
		if ((position.y > 0) && (tileset[tiles[position.x][position.y].tile_id].hasUpPath()))
		{
			addToZone(sf::Vector2i(position.x, position.y - 1), zone_id);
		}
		if ((position.y < (size.y - 1)) && (tileset[tiles[position.x][position.y].tile_id].hasDownPath()))
		{
			addToZone(sf::Vector2i(position.x, position.y + 1), zone_id);
		}
		return true;
	}
	return false;
}

/**********************************************************************************************************************/


void MapGenerator::clearZones()
{
	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			tiles[x][y].zone_id = MapGenerator::NoZoneIndex;
		}
	}
}

/**********************************************************************************************************************/


sf::Vector2i MapGenerator::findZonesBorder()
{
	for (unsigned int x = 1; x < size.x - 1; x++)
	{
		for (unsigned int y = 1; y < size.y - 1; y++)
		{
			if (tiles[x][y].zone_id != tiles[x - 1][y].zone_id)
			{
				if (tiles[x - 1][y].zone_id != MapGenerator::EmptyZoneId)
				{
					return sf::Vector2i(x, y);
				}
			}
			if (tiles[x][y].zone_id != tiles[x + 1][y].zone_id)
			{
				if (tiles[x + 1][y].zone_id != MapGenerator::EmptyZoneId)
				{
					return sf::Vector2i(x, y);
				}
			}
			if (tiles[x][y].zone_id != tiles[x][y - 1].zone_id)
			{
				if (tiles[x][y - 1].zone_id != MapGenerator::EmptyZoneId)
				{
					return sf::Vector2i(x, y);
				}
			}
			if (tiles[x][y].zone_id != tiles[x][y + 1].zone_id)
			{
				if (tiles[x][y + 1].zone_id != MapGenerator::EmptyZoneId)
				{
					return sf::Vector2i(x, y);
				}
			}
		}
	}
	return sf::Vector2i(0, 0);
}

/**********************************************************************************************************************/


void MapGenerator::deleteZonesBorder(sf::Vector2i position)
{
	if (tiles[position.x][position.y].zone_id != tiles[position.x - 1][position.y].zone_id)
	{
		if (tiles[position.x - 1][position.y].zone_id != MapGenerator::EmptyZoneId)
		{
			int up = tileset[tiles[position.x][position.y].tile_id].getUpPath();
			int right = tileset[tiles[position.x][position.y].tile_id].getRightPath();
			int down = tileset[tiles[position.x][position.y].tile_id].getDownPath();
			int left = MapGenerator::Way::Exist;
			tiles[position.x][position.y].tile_id = findTile(up, right, down, left);
			up = tileset[tiles[position.x - 1][position.y].tile_id].getUpPath();
			right = MapGenerator::Way::Exist;
			down = tileset[tiles[position.x - 1][position.y].tile_id].getDownPath();
			left = tileset[tiles[position.x - 1][position.y].tile_id].getLeftPath();
			tiles[position.x - 1][position.y].tile_id = findTile(up, right, down, left);
		}
	}
	if (tiles[position.x][position.y].zone_id != tiles[position.x + 1][position.y].zone_id)
	{
		if (tiles[position.x + 1][position.y].zone_id != MapGenerator::EmptyZoneId)
		{
			int up = tileset[tiles[position.x][position.y].tile_id].getUpPath();
			int right = MapGenerator::Way::Exist;
			int down = tileset[tiles[position.x][position.y].tile_id].getDownPath();
			int left = tileset[tiles[position.x][position.y].tile_id].getLeftPath();
			tiles[position.x][position.y].tile_id = findTile(up, right, down, left);
			up = tileset[tiles[position.x + 1][position.y].tile_id].getUpPath();
			right = tileset[tiles[position.x + 1][position.y].tile_id].getRightPath();
			down = tileset[tiles[position.x + 1][position.y].tile_id].getDownPath();
			left = MapGenerator::Way::Exist;
			tiles[position.x + 1][position.y].tile_id = findTile(up, right, down, left);
		}
	}
	if (tiles[position.x][position.y].zone_id != tiles[position.x][position.y - 1].zone_id)
	{
		if (tiles[position.x][position.y - 1].zone_id != MapGenerator::EmptyZoneId)
		{
			int up = MapGenerator::Way::Exist;
			int right = tileset[tiles[position.x][position.y].tile_id].getRightPath();
			int down = tileset[tiles[position.x][position.y].tile_id].getDownPath();
			int left = tileset[tiles[position.x][position.y].tile_id].getLeftPath();
			tiles[position.x][position.y].tile_id = findTile(up, right, down, left);
			up = tileset[tiles[position.x][position.y - 1].tile_id].getUpPath();
			right = tileset[tiles[position.x][position.y - 1].tile_id].getRightPath();
			down = MapGenerator::Way::Exist;
			left = tileset[tiles[position.x][position.y - 1].tile_id].getLeftPath();
			tiles[position.x][position.y - 1].tile_id = findTile(up, right, down, left);
		}
	}
	if (tiles[position.x][position.y].zone_id != tiles[position.x][position.y + 1].zone_id)
	{
		if (tiles[position.x][position.y + 1].zone_id != MapGenerator::EmptyZoneId)
		{
			int up = tileset[tiles[position.x][position.y].tile_id].getUpPath();
			int right = tileset[tiles[position.x][position.y].tile_id].getRightPath();
			int down = MapGenerator::Way::Exist;
			int left = tileset[tiles[position.x][position.y].tile_id].getLeftPath();
			tiles[position.x][position.y].tile_id = findTile(up, right, down, left);
			up = MapGenerator::Way::Exist;
			right = tileset[tiles[position.x][position.y + 1].tile_id].getRightPath();
			down = tileset[tiles[position.x][position.y + 1].tile_id].getDownPath();
			left = tileset[tiles[position.x][position.y + 1].tile_id].getLeftPath();
			tiles[position.x][position.y + 1].tile_id = findTile(up, right, down, left);
		}
	}
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/