#include <queue>
#include <vector>
#include <map>
#include  <utility>
#include  <iostream>
#include  <cstdlib>


int	main(int argc, char *argv[])
{
	std::map<int, int> map;

	map[1] = 2;

	map.erase(2);
	std::cout << time(NULL) << std::endl;
}
