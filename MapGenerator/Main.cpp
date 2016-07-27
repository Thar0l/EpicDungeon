#include "MapGenerator.h"

int main()
{
	srand(time(NULL));
	MapGenerator mapGenerator;
	mapGenerator.createMap(sf::Vector2i(8, 12));
	mapGenerator.generate();
	mapGenerator.dbg_Print();
	mapGenerator.save("map.txt");
	getchar();
	return 0;
}