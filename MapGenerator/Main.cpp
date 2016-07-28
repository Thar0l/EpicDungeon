#include "MapGenerator.h"

int main()
{
	srand(time(NULL));
	MapGenerator mapGenerator;
	sf::Vector2i size;
	std::cout << "Enter map size.x: ";
	std::cin >> size.x;
	std::cout << "Enter map size.y: ";
	std::cin >> size.y;
	mapGenerator.createMap(size);
	mapGenerator.generate();
	mapGenerator.dbg_Print();
	mapGenerator.saveMap("map.txt");
	std::cout << "Show map (y/n)?" << std::endl;
	char answer;
	std::cin >> answer;
	if (answer == 'y')
	{
		system("MapViewer.exe");
	}
	//system("pause");
	return 0;
}