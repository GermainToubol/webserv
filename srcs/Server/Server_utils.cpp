/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:53:59 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 20:23:58 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "Request.hpp"

#include <sys/stat.h>

bool	WebServer::doesPathExist(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	return (true);
}

bool	WebServer::isPathReadable(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	if (buf.st_mode & S_IRUSR)
		return (true);
	return (false);
}

bool	WebServer::isDirectory(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	if (S_ISDIR(buf.st_mode))
		return (true);
	return (false);
}

bool	WebServer::isFile(std::string const& path)
{
	struct stat		buf;

	if (stat(path.c_str(), &buf) == -1)
		return (false);
	if (S_ISREG(buf.st_mode))
		return (true);
	return (false);
}

int	WebServer::is_server(int fd)
{
	for (std::vector<VirtualServer>::iterator it = this->_virtual_servers.begin(); it != this->_virtual_servers.end(); it++)
	{
		if (it->getFd() == fd)
			return (fd);
	}
	return (-1);
}

Request	*WebServer::get_fd_request(int fd)
{
	size_t	size = this->_all_request.size();

	for (std::vector<Request>::iterator it = this->_all_request.begin(); it != this->_all_request.end(); it++)
	{
		if (it->getFd() == fd)
			return (&(*it));
	}
	
	Request	new_request(fd);
	this->_all_request.push_back(new_request);
	if (size + 1 != this->_all_request.size())
		return (NULL);
	return (&(*(this->_all_request.end() - 1)));
}

void	WebServer::remove_fd_request(int fd)
{
	for (std::vector<Request>::iterator it = this->_all_request.begin(); it != this->_all_request.end(); it++)
	{
		if (it->getFd() == fd)
		{
			this->_all_request.erase(it);
			return ;
		}
	}
}

