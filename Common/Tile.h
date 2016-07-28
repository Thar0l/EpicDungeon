#pragma once


#include <fstream>
#include <iostream>
#include <string>

#include <SFML\Graphics.hpp>

class Tile
{
/******************************************************************************/

private:
	const std::string Path = "Resources\\";
	const std::string Prefix = "Dungeon Tile ";
	const std::string Extension = "txt";

	enum Pathes {
		Up = 0,
		Right = 1,
		Down = 2,
		Left = 3,
		Count = 4
	};
/******************************************************************************/

private:
	int id;
	int pathes[4];
	int rotation;
	sf::Vector2i size;
/******************************************************************************/

public:
	Tile(int id);
	Tile(int id, int rotation);
	Tile& operator=(const Tile& tile);
	bool hasUpPath();
	bool hasRightPath();
	bool hasDownPath();
	bool hasLeftPath();
	sf::Vector2i getSize();
	int getId();
	int getRotation();
	~Tile();
/******************************************************************************/

private:
	void load();
};
