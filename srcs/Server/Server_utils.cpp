/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:53:59 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/28 19:57:31 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include <sys/stat.h>

#include "Request.hpp"
#include "WebServer.hpp"

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

int	WebServer::isMe(std::string const& uri, std::string const& path, std::string const& host)
{
	std::string::size_type	pos = 0;
	std::string				tmp;

	pos = uri.find(host);
	if (pos != std::string::npos)
		pos += host.size();
	else
		pos = 0;
	if (&uri[pos] == path)
		return (true);
	return (false);
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

void	WebServer::clearCache(void)
{
	for (std::vector<Cache>::iterator it = this->_all_cache.begin(); it != this->_all_cache.end(); it++)
	{
		if (it->getUsers() == 0)
		{
			it->getStream()->close();
			delete it->getStream();
			this->_all_cache.erase(it);
			return ;
		}
	}
}

