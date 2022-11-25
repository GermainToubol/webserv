/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:36:53 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/24 21:31:21 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <vector>

#include <stdio.h>

#include "Request.hpp"

Request::Request(int fd):	_fd(fd), _boundary(""), _content(""),
							_method(""), _uri(""),
							_version(""), _body("") {}

Request::~Request() {}

int	Request::addContent(std::string const& content)
{
	this->_content += content;
	if (content.find("\r\n\r\n") != std::string::npos)
		return (1);
	return (0);
}

int	Request::setFirstline(Setup *setup, std::string const& line)
{
	std::string::size_type	pos = 0;
	std::string::size_type	pos2 = 0;

	pos = line.find(" ");
	if (pos == std::string::npos)
		return (perror("/!\\ No Method"), setup->setCode(400), 400);
	this->_method = line.substr(0, pos);
	pos2 = line.find(" ", pos + 1);
	if (pos2 == std::string::npos)
		return (perror("/!\\ No Uri"), setup->setCode(400), 400);
	this->_uri = line.substr(pos + 1, pos2 - pos - 1);
	pos = this->_uri.find("?");
	if (pos != std::string::npos)
	{
		this->_query = this->_uri.substr(pos + 1);
		this->_uri.erase(pos);
	}
	this->_version = line.substr(pos2 + 1, line.size() - pos2 - 1);
	if (this->_version != "HTTP/1.1")
		return (perror("/!\\ Bad HTTP version"), setup->setCode(505), 505);
	return (0);	
}

int	Request::setUri(Setup *setup)
{
	if (this->_uri == this->_location->getRoot())
		this->_uri = this->_location->getIndex();
	else
		this->_uri.replace(0, this->_location_path.size(), this->_location->getRoot());
	this->_uri = setup->getServer().getRoot() + this->_uri;
	return (0);
}

int	Request::getLocation(Setup *setup)
{
	std::map<std::string, Location> const&	location_pool = setup->getServer().getLocationPool();
	std::string				tmp;
	std::string::size_type	pos = 0;
	
	tmp = this->_uri;
	pos = tmp.find_last_of("/");
	while (1)
	{
		pos = tmp.find_last_of("/");
		if (pos == std::string::npos)
			break ;
		if (pos == tmp.size() - 1)
			tmp.erase(pos);
		else
			tmp.erase(pos + 1);
		std::cout << tmp << std::endl;
		if (location_pool.find(tmp) != location_pool.end())
		{
			std::cerr << "[ location: " << location_pool.find(tmp)->first << " ]" << std::endl;
			this->_location = &location_pool.find(tmp)->second;
			this->_location_path = tmp;
			return (0);
		}
	}
	this->_location = &location_pool.find("/")->second;
	this->_location_path = "/";
	return (0);
}

int	Request::getServer(Setup *setup, std::vector<VirtualServer> const& server_pool)
{
	std::string	host;
	
	if (this->_fields.find("Host") == this->_fields.end())
		return (perror("/!\\ No Host"), setup->setCode(400), 400);
	host = this->_fields["Host"];
	if (host.find(":") != std::string::npos)
		host = host.substr(0, host.find(":"));
	for (std::vector<VirtualServer>::const_iterator it = server_pool.begin(); it != server_pool.end(); ++it)
	{
		if (it->getServerName() == host)
			return (setup->setServer(*it), 0);
	}
	if (server_pool.size() < 1)
		return (setup->setServer(server_pool[0]), setup->setCode(400), 400);
	return (setup->setServer(server_pool[0]), 0);
}

int	Request::parsing(Setup *setup)
{
	std::string::size_type	pos;
	std::string				line;
	int						ret;

	pos = this->_content.find("\r\n\r\n");
	if (pos != std::string::npos)
	{
		this->_body = this->_content.substr(pos + 4);
		this->_content.erase(pos + 2);
	}
	pos = this->_content.find("\r\n");
	if (pos == std::string::npos)
		return (perror("/!\\ Empty header"), setup->setCode(400), 400);
	line = this->_content.substr(0, this->_content.find("\r\n"));
	this->_content.erase(0, this->_content.find("\r\n") + 2);
	ret = this->setFirstline(setup, line);
	if (ret)
		return (ret);
	for (pos = this->_content.find("\r\n"); pos != std::string::npos; pos = this->_content.find("\r\n"))
	{
		line = this->_content.substr(0, pos);
		this->_content.erase(0, pos + 2);
		if (line.find(": ") == std::string::npos)
			return (perror("/!\\ bad syntax on header field"), setup->setCode(400), 400);
		this->_fields[line.substr(0, line.find(": "))] = line.substr(line.find(": ") + 2);
	}
	std::cerr << "[ Parsed request ]" << std::endl;
	std::cerr << "Method: " << this->_method << std::endl;
	std::cerr << "URI: " << this->_uri << std::endl;
	std::cerr << "Version: " << this->_version << std::endl;
	std::cerr << "Body: " << this->_body << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->_fields.begin(); it != this->_fields.end(); it++)
		std::cerr << it->first << ": " << it->second << std::endl;
	std::cerr << "[ End Parsed request ]" << std::endl;
	return (0);
}

/*Accesseurs*/
std::string const&	Request::getBoundary() const
{
	return (this->_boundary);
}

std::string const&	Request::getContent() const
{
	return (this->_content);
}

int	const&	Request::getFd() const
{
	return (this->_fd);
}

void	Request::setBoundary(std::string const& boundary)
{
	this->_boundary = boundary;
}

void	Request::setContent(std::string const& content)
{
	this->_content = content;
}

void	Request::setFd(int const& fd)
{
	this->_fd = fd;
}
