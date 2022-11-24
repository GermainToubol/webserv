/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Setup.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 20:13:01 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/24 13:13:08 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Setup.hpp"

Setup::Setup(): _code(0), _uri(""), _fields(""), _server(0) {}

Setup::~Setup() {}

int		const&	Setup::getCode() const
{
	return (this->_code);
}

std::string const&	Setup::getUri() const
{
	return (this->_uri);
}

std::string const&	Setup::getFields() const
{
	return (this->_fields);
}

VirtualServer const&	Setup::getServer() const
{
	return (*this->_server);
}

void	Setup::setCode(int const& code)
{
	this->_code = code;
}

void	Setup::setUri(std::string const& uri)
{
	this->_uri = uri;
}

void	Setup::setFields(std::string const& fields)
{
	this->_fields = fields;
}

void	Setup::setServer(VirtualServer const& server)
{
	this->_server = &server;
}
