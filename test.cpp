#include <map>
#include <string>
#include <iostream>

#include <sys/stat.h>
#include <dirent.h>

bool	doesPathExist(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	return (true);
}

bool	isPathReadable(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	if (buf.st_mode & S_IRUSR)
		return (true);
	return (false);
}

bool	isDirectory(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	if (S_ISDIR(buf.st_mode))
		return (true);
	return (false);
}

bool	isFile(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	if (S_ISREG(buf.st_mode))
		return (true);
	return (false);
}

int	createHtmlFromDirectory(std::string path)
{
	DIR				*dir;
	struct dirent	*ent;
	std::string		html;

	//need to cut root from path and set / at end
	if (*(path.end() - 1) != '/')
		path += '/';
	html = "<!DOCTYPE html>\n\
<html>\n<head>\n<title>Index of " + path + "</title>\n</head>\n\
<body>\n<h1 style=\"font-size:30px\">Index of " + path + "</h1>\n<br><br><hr>\n";
	dir = opendir(path.c_str());
	if (dir == NULL)
		return (-1);
	while ((ent = readdir(dir)) != NULL)
		html += "<a style=\"margin: 5px; font-size: 20px; font-style: italic;\" href=\"" + path + ent->d_name + "\">" + ent->d_name + "</a><br><hr>\n";
	html += "</body>\n</html>";
	closedir(dir);
	std::cout << html << std::endl;
	return (0);
}

int	main(int argc, char *argv[])
{
	//std::cout << doesPathExist(argv[argc - 1]) << std::endl;
	//std::cout << isPathReadable(argv[argc - 1]) << std::endl;
	//std::cout << isDirectory(argv[argc - 1]) << std::endl;
	//std::cout << isFile(argv[argc - 1]) << std::endl;
	if (doesPathExist(argv[argc - 1]) && isPathReadable(argv[argc - 1]) && isDirectory(argv[argc - 1]))
		createHtmlFromDirectory(argv[argc - 1]);
}
