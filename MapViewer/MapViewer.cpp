#include "MapViewer.h"



MapViewer::MapViewer()
{
	window.create(sf::VideoMode(1920, 1080), "MapViewer");
}

/**********************************************************************************************************************/


void MapViewer::loadMap(std::string filename)
{
	std::ifstream file;
	file.open(filename, std::ifstream::in);
	file >> size.x >> size.y;
	//std::cout << size.x << "" << size.y << std::endl;

	for (unsigned int x = 0; x < size.x; x++)
	{
		std::vector<int> line;
		std::vector<sf::Texture> textures_line;
		std::vector<sf::Sprite> sprites_line;
		for (unsigned int y = 0; y < size.y; y++)
		{
			sf::Texture texture;
			sf::Sprite sprite;
			line.push_back(MapViewer::EmptyTileId);
			textures_line.push_back(texture);
			sprites_line.push_back(sprite);
		}
		tiles.push_back(line);
		textures.push_back(textures_line);
		sprites.push_back(sprites_line);
	}

	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			file >> tiles[x][y];
		}
	}
	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			int rotation;
			file >> rotation;
			tiles[x][y] = addTile(tiles[x][y], rotation);
			textures[x][y].loadFromFile(MapViewer::Path + MapViewer::Prefix + std::to_string(tileset[tiles[x][y]].getId()) + ".jpg");
			sprites[x][y].setTexture(textures[x][y]);
			sprites[x][y].setPosition(1200 * x, 1200 * y);
			sprites[x][y].setOrigin(600, 600);
			sprites[x][y].setRotation(90 * tileset[tiles[x][y]].getRotation());
		}
	}
	file.close();
}

/**********************************************************************************************************************/


MapViewer::~MapViewer()
{
}

/**********************************************************************************************************************/


void MapViewer::dbg_Print()
{
	std::cout << std::endl << "dbg_map_tiles_ids:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		for (unsigned int x = 0; x < size.x; x++)
		{
			std::cout << tileset[tiles[x][y]].getId() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "dbg_map_tiles_rotations:" << std::endl;
	for (unsigned int y = 0; y < size.y; y++)
	{
		std::vector<Tile> line;
		for (unsigned int x = 0; x < size.x; x++)
		{
			std::cout << tileset[tiles[x][y]].getRotation() << " ";
		}
		std::cout << std::endl;
	}
}

/**********************************************************************************************************************/


void MapViewer::Show()
{
	sf::View view = window.getDefaultView();
	view.zoom(20.0f);
	window.setView(view);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();
		for (unsigned int y = 0; y < size.y; y++)
		{
			for (unsigned int x = 0; x < size.x; x++)
			{
				window.draw(sprites[x][y]);
			}
		}
		window.display();
	}
}

/**********************************************************************************************************************/


int MapViewer::addTile(int id, int rotation)
{
	Tile tile(id, rotation);
	tileset.push_back(tile);
	return tileset.size() - 1;
}