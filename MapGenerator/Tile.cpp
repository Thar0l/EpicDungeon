#include "Tile.h"

/**********************************************************************************************************************/


Tile::Tile(int id)
{
	this->id = id;
	this->size.x = 6;
	this->size.y = 6;
}

/**********************************************************************************************************************/


Tile::Tile(int id, int rotation)
{
	this->id = id;
	this->rotation = rotation;
}

/**********************************************************************************************************************/


bool Tile::hasUpPath()
{
	return pathes[(Tile::Pathes::Count + Tile::Pathes::Up - rotation) % Tile::Pathes::Count];
}

/**********************************************************************************************************************/

bool Tile::hasRightPath()
{
	return pathes[(Tile::Pathes::Count + Tile::Pathes::Right - rotation) % Tile::Pathes::Count];
}


/**********************************************************************************************************************/

bool Tile::hasDownPath()
{
	return pathes[(Tile::Pathes::Count + Tile::Pathes::Down - rotation) % Tile::Pathes::Count];
}


/**********************************************************************************************************************/

bool Tile::hasLeftPath()
{
	return pathes[(Tile::Pathes::Count + Tile::Pathes::Left - rotation) % Tile::Pathes::Count];
}


/**********************************************************************************************************************/

sf::Vector2i Tile::getSize()
{
	return size;
}

/**********************************************************************************************************************/


Tile::~Tile()
{
}

/**********************************************************************************************************************/

void Tile::Load()
{
	std::ifstream file;
	file.open(Tile::Path + Tile::Prefix + std::to_string(id) + "." + Tile::Extension, std::ifstream::in);
	file >> size.x >> size.y;
	for (unsigned int path_id = 0; path_id < Tile::Pathes::Count; path_id++)
	{
		file >> pathes[path_id];
	}
	file.close();
}
