#include <queue>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include  <utility>
#include  <iostream>
#include  <cstdlib>

std::string	reformatUri(std::string const& uri)
{
	std::string				new_uri;
	std::string::size_type	start = 0;
	std::string::size_type	end = 0;
	std::stack<std::string>	stack;
	std::string				tmp;

	while (end != uri.size())
	{
		if (start != 0)
			start = uri.find('/', end);
		while (uri.find('/', start + 1) == 0)
			start++;
		if (uri[start] == '/')
			start++;
		end = uri.find('/', start);
		if (end == std::string::npos)
			end = uri.size();
		std::cerr << "start: " << start << " end: " << end << std::endl;
		tmp = uri.substr(start, end - start);
		if (tmp == "..")
		{
			if (!stack.empty())
				stack.pop();
		}
		else if (tmp != "." && tmp != "")
			stack.push(tmp);
		start = end;
	}
	while (!stack.empty())
	{
		new_uri = stack.top() + "/" + new_uri;
		stack.pop();
	}
	if (uri[0] == '/')
		new_uri = "/" + new_uri;
	if (uri[uri.size() - 1] == '/' && new_uri[new_uri.size() - 1] != '/')
		new_uri = new_uri + "/";
	else if (uri[uri.size() - 1] != '/' && new_uri[new_uri.size() - 1] == '/')
		new_uri = new_uri.erase(new_uri.size() - 1);
	if (new_uri.size() == 0)
		new_uri = "/";
	return (new_uri);
}

int	main(int argc, char *argv[])
{
	std::string	uri = "ergfwe/feojfpe//../fefje/./../febfkjf;oj/fej;fje/fehfle//ono/ooui";

	std::cout << reformatUri(uri) << std::endl;
}
