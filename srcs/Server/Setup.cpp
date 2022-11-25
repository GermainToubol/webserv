/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Setup.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 20:13:01 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 15:03:09 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Setup.hpp"

Setup::Setup(): _code(0), _uri(""), _fields(""), _server(0) {}

Setup::~Setup() {}

int	Setup::addField(std::string const& key, std::string const& value)
{
	this->_fields += key + ": " + value + "\r\n";
	return (0);
}

int	Setup::eraseUri(std::string::size_type pos)
{
	this->_uri.erase(pos);
	return (0);
}

int	Setup::replaceUri(std::string::size_type pos, std::string::size_type size, std::string const& str)
{
	this->_uri.replace(pos, size, str);
	return (0);
}

/*Accesseurs*/
int		const&	Setup::getCode() const
{
	return (this->_code);
}

std::string const&	Setup::getUri() const
{
	return (this->_uri);
}

std::string const&	Setup::getQuery() const
{
	return (this->_query);
}

std::string const&	Setup::getExtension() const
{
	return (this->_extension);
}

std::string const&	Setup::getFields() const
{
	return (this->_fields);
}

VirtualServer const	*Setup::getServer() const
{
	return (this->_server);
}

void	Setup::setCode(int const& code)
{
	this->_code = code;
}

void	Setup::setUri(std::string const& uri)
{
	this->_uri = uri;
}

void	Setup::setUri(int code)
{
	if (this->_server->getErrorPage().find(code) != this->_server->getErrorPage().end())
		this->_uri = this->_server->getErrorPage().find(code)->second;
	else
		this->_uri = "";
}

void	Setup::setQuery(std::string const& query)
{
	this->_query = query;
}

void	Setup::setExtension(std::string const& extension)
{
	this->_extension = extension;
}

void	Setup::setExtension(void)
{
	std::string::size_type	pos;
	pos = this->_uri.find_last_of("/");
	if (pos != std::string::npos)
	{
		pos = this->_uri.find_last_of(".", pos);
		if (pos != std::string::npos)
			this->_extension = this->_uri.substr(pos + 1);
		else
			this->_extension = "";
	}
	else
		this->_extension = "";
}

void	Setup::setFields(std::string const& fields)
{
	this->_fields = fields;
}

void	Setup::setServer(VirtualServer server)
{
	this->_server = &server;
}

void	Setup::setServer(VirtualServer *server)
{
	this->_server = server;
}
