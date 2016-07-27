#include "MapGenerator.h"

int main()
{
	srand(time(NULL));
	MapGenerator mapGenerator;
	mapGenerator.createMap(sf::Vector2i(5, 5));
	mapGenerator.generate();
	mapGenerator.dbg_Print();
	mapGenerator.saveMap("map.txt");
	getchar();
	return 0;
}