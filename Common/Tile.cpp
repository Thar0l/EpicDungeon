#include "Tile.h"

/**********************************************************************************************************************/


Tile::Tile(int id)
{
	this->id = id;
	this->size.x = 6;
	this->size.y = 6;
	this->rotation = 0;
	load();
}

/**********************************************************************************************************************/


Tile::Tile(int id, int rotation)
{
	this->id = id;
	this->rotation = rotation;
	load();
}

/**********************************************************************************************************************/


Tile& Tile::operator=(const Tile& tile)
{
	this->id = tile.id;
	this->rotation = tile.rotation;
	this->load();
	return Tile(tile.id, tile.rotation);
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


int Tile::getId()
{
	return id;
}

/**********************************************************************************************************************/


int Tile::getRotation()
{
	return rotation;
}

/**********************************************************************************************************************/


Tile::~Tile()
{
}

/**********************************************************************************************************************/


void Tile::load()
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
