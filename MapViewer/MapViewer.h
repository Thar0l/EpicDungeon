#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <SFML\Graphics.hpp>

#include "..\Common\Tile.h"

class MapViewer
{
private:
	const std::string Path = "Resources\\";
	const std::string Prefix = "Dungeon Tile ";
	const std::string TilesetName = "Rooms";
	const std::string Extension = "txt";
	const int EmptyTileId = 0;

private:
	std::string filename;
	
	sf::RenderWindow window;
	std::vector<sf::Texture> textures;
	std::vector<std::vector<sf::Sprite>> sprites;

	
	sf::Vector2i size;
	std::vector<Tile> tileset;
	std::vector<std::vector<int>> tiles;

	float zoom;
	sf::Vector2f mapPosition;
	sf::Vector2f mousePosition;

public:
	MapViewer();
	void loadMap(std::string filename);
	~MapViewer();
	void dbg_Print();
	void Show();
private:
	int addTile(int id, int rotation);
	void loadTileset();
};
