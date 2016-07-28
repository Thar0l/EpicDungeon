#include "MapViewer.h"



MapViewer::MapViewer()
{
	window.create(sf::VideoMode(1920, 1080), "MapViewer");
	zoom = 2.0f;
	mapPosition = sf::Vector2f(0.0, 0.0);
}

/**********************************************************************************************************************/


void MapViewer::loadMap(std::string filename)
{
	std::ifstream file;
	file.open(filename, std::ifstream::in);
	file >> size.x >> size.y;
	for (unsigned int x = 0; x < size.x; x++)
	{
		std::vector<int> line;
		std::vector<sf::Sprite> sprites_line;
		for (unsigned int y = 0; y < size.y; y++)
		{
			sf::Sprite sprite;
			line.push_back(MapViewer::EmptyTileId);
			sprites_line.push_back(sprite);
		}
		tiles.push_back(line);
		sprites.push_back(sprites_line);
	}

	loadTileset();

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
			sprites[x][y].setTexture(textures[tiles[x][y]]);
			sprites[x][y].setPosition(1200 * x, 1200 * y);
			sprites[x][y].setOrigin(600, 600);
			sprites[x][y].setRotation(90 * rotation);
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
	view.zoom(zoom);
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
			if (event.type == sf::Event::MouseWheelMoved)
			{
				zoom += (float)(event.mouseWheel.delta);
				if (zoom < 1.0)
				{
					zoom = 1.0;
				}
				if (zoom > 25.0)
				{
					zoom = 25.0;
				}
				sf::View view = window.getDefaultView();
				view.move(mapPosition);
				view.zoom(zoom);
				window.setView(view);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::R)
				{
					sf::View view = window.getDefaultView();
					mapPosition = sf::Vector2f(0.0, 0.0);
					zoom = 2.0f;
					view.move(mapPosition);
					view.zoom(zoom);
					window.setView(view);
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					sf::View view = window.getDefaultView();
					mapPosition.y -= 100;
					view.move(mapPosition);
					view.zoom(zoom);
					window.setView(view);
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					sf::View view = window.getDefaultView();
					mapPosition.y += 100;
					view.move(mapPosition);
					view.zoom(zoom);
					window.setView(view);
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					sf::View view = window.getDefaultView();
					mapPosition.x -= 100;
					view.move(mapPosition);
					view.zoom(zoom);
					window.setView(view);
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					sf::View view = window.getDefaultView();
					mapPosition.x += 100;
					view.move(mapPosition);
					view.zoom(zoom);
					window.setView(view);
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					mousePosition.x = sf::Mouse::getPosition(window).x;
					mousePosition.y = sf::Mouse::getPosition(window).y;
				}
			}

			if (event.type == sf::Event::MouseMoved)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					sf::Vector2f position;
					position.x = mousePosition.x - sf::Mouse::getPosition(window).x;
					position.y = mousePosition.y - sf::Mouse::getPosition(window).y;
					sf::View view = window.getDefaultView();
					view.move(mapPosition.x + zoom *position.x, mapPosition.y + zoom * position.y);
					view.zoom(zoom);
					window.setView(view);
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					mousePosition.x = mousePosition.x - sf::Mouse::getPosition(window).x;
					mousePosition.y = mousePosition.y - sf::Mouse::getPosition(window).y;
					sf::View view = window.getDefaultView();
					mapPosition.x += zoom * mousePosition.x;
					mapPosition.y += zoom * mousePosition.y;
					view.move(mapPosition);
					view.zoom(zoom);
					window.setView(view);
				}
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

void MapViewer::loadTileset()
{
	int tiles_count;
	std::ifstream file;
	file.open(MapViewer::Path + MapViewer::TilesetName + "." + MapViewer::Extension, std::ifstream::in);
	if (file.fail())
	{
		std::cout << "No resources found!" << std::endl;
		system("pause");
		exit(-1);
	}
	file >> tiles_count;
	for (unsigned int tile_id = 0; tile_id < tiles_count; tile_id++)
	{

		Tile tile(tile_id, 0);
		tileset.push_back(tile);
		sf::Texture texture;
		texture.loadFromFile(MapViewer::Path + MapViewer::Prefix + std::to_string(tile_id) + ".jpg");
		textures.push_back(texture);
	}
	file.close();
}