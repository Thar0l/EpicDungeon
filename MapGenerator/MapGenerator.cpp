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
		std::vector<int> line;
		for (unsigned int y = 0; y < size.y; y++)
		{
			line.push_back(MapGenerator::NoTileIndx);
		}
		tiles.push_back(line);
	}
	for (unsigned int x = 0; x < size.x; x++)
	{
		tiles[x][0] = MapGenerator::EmptyTileId;
		tiles[x][size.y - 1] = MapGenerator::EmptyTileId;
	}
	for (unsigned int y = 0; y < size.y; y++)
	{	
		tiles[0][y] = MapGenerator::EmptyTileId;
		tiles[size.x - 1][y] = MapGenerator::EmptyTileId;
	}
}

/**********************************************************************************************************************/


void MapGenerator::generate()
{
	sf::Vector2i position = genFirstTile();
	genNeighborTiles(position);
}

/**********************************************************************************************************************/


void MapGenerator::dbg_Print()
{
	std::cout << std::endl << "dbg_map_tiles_ids:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			if (tiles[x][y] == MapGenerator::NoTileIndx)
			{
				std::cout << "-" << " ";
			}
			else
			{
				std::cout << tileset[tiles[x][y]].getId() << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "dbg_map_tiles_rotations:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		std::vector<Tile> line;
		for (unsigned int x = 0; x < size.x; x++)
		{
			if (tiles[x][y] == MapGenerator::NoTileIndx)
			{
				std::cout << "-" << " ";
			}
			else
			{
				std::cout << tileset[tiles[x][y]].getRotation() << " ";
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
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			file << tileset[tiles[x][y]].getId() << " ";
		}
		file << std::endl;
	}
	file << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			file << tileset[tiles[x][y]].getRotation() << " ";
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
	sf::Vector2i position = sf::Vector2i((rand() % (size.x - 4) + 2), (rand() % (size.y - 4) + 2));
	tiles[position.x][position.y] = tileset[rand() % (tileset.size() - 1) + 1].getId();
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
	if (tiles[position.x][position.y] == MapGenerator::NoTileIndx)
	{
		int up;
		int right;
		int down;
		int left;
		
		if (tiles[position.x][position.y-1] == MapGenerator::NoTileIndx)
		{
			up = MapGenerator::Way::Possible;
		} 
		else
		{
			if (tileset[tiles[position.x][position.y-1]].hasDownPath())
			{
				up = MapGenerator::Way::Exist;
			}
			else
			{
				up = MapGenerator::Way::Absent;
			}
		}

		if (tiles[position.x + 1][position.y] == MapGenerator::NoTileIndx)
		{
			right = MapGenerator::Way::Possible;
		}
		else
		{
			if (tileset[tiles[position.x + 1][position.y]].hasLeftPath())
			{
				right = MapGenerator::Way::Exist;
			}
			else
			{
				right = MapGenerator::Way::Absent;
			}
		}

		if (tiles[position.x][position.y + 1] == MapGenerator::NoTileIndx)
		{
			down = MapGenerator::Way::Possible;
		}
		else
		{
			if (tileset[tiles[position.x][position.y + 1]].hasUpPath())
			{
				down = MapGenerator::Way::Exist;
			}
			else
			{
				down = MapGenerator::Way::Absent;
			}
		}

		if (tiles[position.x - 1][position.y] == MapGenerator::NoTileIndx)
		{
			left = MapGenerator::Way::Possible;
		}
		else
		{
			if (tileset[tiles[position.x - 1][position.y]].hasRightPath())
			{
				left = MapGenerator::Way::Exist;
			}
			else
			{
				left = MapGenerator::Way::Absent;
			}
		}
#ifdef DEBUG_MODE
		std::cout << "Finding tile for place (" << position.x << "; " << position.y << ")." << std::endl;
		std::cout << "Params is: " << up << " " << right << " " << down << " " << left << "." << std::endl;
#endif
		tiles[position.x][position.y] = findTile(up, right, down, left);
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
		if ((up != MapGenerator::Way::Possible) && (tileset[index].hasUpPath() != up))
		{
			continue;
		}
		if ((right != MapGenerator::Way::Possible) && (tileset[index].hasRightPath() != right))
		{
			continue;
		}
		if ((down != MapGenerator::Way::Possible) && (tileset[index].hasDownPath() != down))
		{
			continue;
		}
		if ((left != MapGenerator::Way::Possible) && (tileset[index].hasLeftPath() != left))
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