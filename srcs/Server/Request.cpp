/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:36:53 by lgiband           #+#    #+#             */
/*   Updated: 2022/12/02 11:44:32 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <stdio.h>
#include <sys/stat.h>

#include "Request.hpp"
#include "utils.hpp"

Request::Request(int fd):	_fd(fd), _boundary(""), _content(""), _content_size(-1), _is_header(0),
							_method(""), _uri(""),
							_version(""), _body(""), _empty("") {}

Request::~Request() {}

int	Request::addContent(char *buf, int ret)
{
	std::string::size_type	pos;
	std::string::size_type	pos2;

	this->_content += std::string(buf, ret);
	std::cerr << "[ Ret: " << ret << " ]" << std::endl;
	std::cerr << "[ end header: " << this->_content.find("\r\n\r\n") << " ]" << std::endl;
	if (ret == 0)
		return (1);
	if (this->_content.find("\r\n\r\n") != std::string::npos && this->_is_header == 0)
	{
		pos = this->_content.find("Content-Length: ");
		if (pos == std::string::npos)
			return (1);
		pos2 = this->_content.find("\r\n", pos); //check overflow
		this->_content_size = std::strtol(this->_content.substr(pos + 16, pos2 - pos - 16).c_str(), NULL, 10);
		this->_is_header = 1;
		std::cerr << "content size: " << this->_content_size << std::endl;
		if (this->_content_size <= 0)
			return (1);
	}
	if (this->_content.find("\r\n\r\n") != std::string::npos && this->_is_header == 1)
	{
		std::cerr << "content size after: " << this->_content_size << std::endl;
		pos = this->_content.find("\r\n\r\n");
		pos2 = this->_content.size() - pos - 4;
		std::cerr << "pos2: " << pos2 << " max: " << this->_content_size << std::endl;
		if (pos2 >= (unsigned int)this->_content_size)
			return (derror("normal end"), 1);
	}
	return (0);
}

int	Request::setFirstline(Setup *setup, std::string const& line)
{
	std::string::size_type	pos = 0;
	std::string::size_type	pos2 = 0;

	pos = line.find(" ");
	if (pos == std::string::npos)
		return (derror("/!\\ No Method"), setup->setCode(400), 400);
	this->_method = line.substr(0, pos);
	pos2 = line.find(" ", pos + 1);
	if (pos2 == std::string::npos)
		return (derror("/!\\ No Uri"), setup->setCode(400), 400);
	this->_uri = line.substr(pos + 1, pos2 - pos - 1);
	pos = this->_uri.find("?");
	setup->setUri(this->_uri.substr(0, pos));
	if (pos != std::string::npos)
		setup->setQuery(setup->getUri().substr(pos + 1));
	setup->setExtension();
	this->_version = line.substr(pos2 + 1, line.size() - pos2 - 1);
	if (this->_version != "HTTP/1.1")
		return (derror("/!\\ Bad HTTP version"), setup->setCode(505), 505);
	return (0);	
}

bool	Request::isValidUri(std::string const& uri)
{
	int	total = 0;
	int current = 0;

	for (std::string::const_iterator it = uri.begin(); it != uri.end(); it++)
	{
		if (*it == '/')
			current = 0;
		else
			current++;
		total++;
		if (current > 255 || total > 4095)
			return (false);
	}
	return (true);
}

int	Request::basicCheck(Setup *setup)
{
	if (this->_version != "HTTP/1.1")
		return (derror("/!\\ Bad HTTP version"), setup->setCode(505), 505);
	if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
		return (derror("/!\\ Bad Method"), setup->setCode(405), 405);
	if (!this->isValidUri(setup->getUri()))
		return (derror("/!\\ Bad Uri"), setup->setCode(414), 414);
	return (0);
}

int	Request::setUri(Setup *setup)
{
	
	if (setup->getUri() == "/" && this->_method == "GET")
		setup->setUri(this->_location->getIndex());
	
	setup->setUri(uriDecode(reformatUri(setup->getUri())));
	setup->replaceUri(0, this->_location_path.size(), this->_location->getRoot());

	if (isDirectory(setup->getUri()) && setup->getUri()[setup->getUri().size() - 1] != '/')
		setup->addUri("/");
	std::cerr << "uri: " << setup->getUri() << std::endl;
	return (0);
}

int	Request::setLocation(Setup *setup)
{
	std::map<std::string, Location>	const&	location_pool = setup->getServer()->getLocationPool();
	std::string				tmp;
	std::string::size_type	pos;
	
	tmp = setup->getUri();
	if (setup->getUri()[setup->getUri().size() - 1] != '/')
		tmp += "/";
	while (1)
	{
		std::cout << "[ tmp: " << tmp << " ]" << std::endl;
		if (location_pool.find(tmp) != location_pool.end())
		{
			std::cerr << "[ location: " << location_pool.find(tmp)->first << " ]" << std::endl;
			this->_location = &location_pool.find(tmp)->second;
			this->_location_path = tmp;
			return (0);
		}
		pos = tmp.find_last_of("/");
		if (pos == std::string::npos)
			break ;
		if (pos == tmp.size() - 1)
			tmp.erase(pos);
		else
			tmp.erase(pos + 1);
	}
	this->_location = &location_pool.find("/")->second;
	this->_location_path = "/";
	return (0);
}

int	Request::setServer(Setup *setup, std::vector<VirtualServer*> const* server_pool)
{
	std::string	host;

	if (!server_pool)
		return (derror("/!\\ No Server"), setup->setCode(500), 500);
	if (this->_fields.find("Host") == this->_fields.end())
		return (derror("/!\\ No Host"), setup->setCode(400), 400);
	host = this->_fields["Host"];
	if (host.find(":") != std::string::npos)
		host = host.substr(0, host.find(":"));
	for (std::vector<VirtualServer *>::const_iterator it = server_pool->begin(); it != server_pool->end(); ++it)
	{
		if (*it && (*it)->getServerName() == host)
			return (setup->setServer(*it), 0);
	}
	if (server_pool->size() < 1)
		return (setup->setServer(NULL), setup->setCode(400), 400);
	return (setup->setServer(*server_pool->begin()), 0);
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
		return (derror("/!\\ Empty header"), setup->setCode(400), 400);
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
			return (derror("/!\\ bad syntax on header field"), setup->setCode(400), 400);
		this->_fields[line.substr(0, line.find(": "))] = line.substr(line.find(": ") + 2);
	}
	std::cerr << "[ Parsed request ]" << std::endl;
	std::cerr << "Method: " << this->_method << std::endl;
	std::cerr << "URI: " << this->_uri << std::endl;
	std::cerr << "Version: " << this->_version << std::endl;
	std::cerr << "Body size: " << this->_body.size() << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->_fields.begin(); it != this->_fields.end(); it++)
		std::cerr << it->first << ": " << it->second << std::endl;
	std::cerr << "[ End Parsed request ]" << std::endl;
	setup->setUserSession(this->_fields["Cookie"]);
	return (0);
}

void	Request::replaceAllBody(std::string const& from, std::string const& to)
{
	std::string::size_type	pos;

	while ((pos = this->_body.find(from)) != std::string::npos)
		this->_body.replace(pos, from.size(), to);
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

Location const	*Request::getLocation() const
{
	return (this->_location);
}

std::string const&	Request::getMethod() const
{
	return (this->_method);
}

std::string const&	Request::getExtension() const
{
	return (this->_extension);
}

std::string const&	Request::getUri() const
{
	return (this->_uri);
}

std::string const&	Request::getField(std::string key) const
{

	if (this->_fields.find(key) != this->_fields.end())
		return (this->_fields.find(key)->second);
	return (this->_empty);
}

std::string const& Request::getBody() const
{
	return (this->_body);
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

void	Request::addBody(char *buffer, int size)
{
	this->_body += std::string(buffer, size);
}
