#include <map>
#include <string>
#include <iostream>

int	main(int argc, char *argv[])
{
	std::map<std::string, std::string> location_pool;
	std::string				tmp;
	std::string::size_type	pos = 0;
	
	location_pool["/"] = "root";
	location_pool["/test"] = "test";
	location_pool["/test/"] = "test/";
	location_pool["/test/test"] = "test/test";
	location_pool["/test/test/"] = "test/test/";
	location_pool["/test/test/test"] = "test/test/test";
	location_pool["/test/test/test/"] = "test/test/test/";

	tmp = argv[argc - 1];
	pos = tmp.find_last_of("/");
	while (pos != std::string::npos)
	{
		if (location_pool.find(tmp) != location_pool.end())
		{
			std::cout << "location: " << location_pool.find(tmp)->second << std::endl;
			return (0);
		}
		tmp.erase(pos+1);
		pos = tmp.find_last_of("/");
		std::cout << tmp << std::endl;
	}
	return (0);
}
