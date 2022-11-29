/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:03:52 by gtoubol           #+#    #+#             */
/*   Updated: 2022/11/25 14:57:39 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>

#include "VirtualServer.hpp"
#include "Location.hpp"

VirtualServer::VirtualServer(void):
	server_name(""),
	root(""),
	host("0.0.0.0"),
	port("")
{
	return ;
}

std::string const& VirtualServer::getServerName() const
{
	return (this->server_name);
}

std::string const& VirtualServer::getRoot() const
{
	return (this->root);
}

std::string const& VirtualServer::getHost() const
{
	return (this->host);
}

std::string const& VirtualServer::getPort() const
{
	return (this->port);
}

int const& VirtualServer::getFd() const
{
	return (this->fd);
}

std::map<std::string, Location> const& VirtualServer::getLocationPool() const
{
	return (this->location_pool);
}

std::map<std::string, Location> &VirtualServer::getLocationPool()
{
	return (this->location_pool);
}

std::map<int, std::string> const& VirtualServer::getErrorPage() const
{
	return (this->error_page);
}

void VirtualServer::setServerName(std::string const& name)
{
	this->server_name = name;
}

void VirtualServer::setRoot(std::string const& directory)
{
	this->root = directory;
}

void VirtualServer::setHost(std::string const& host)
{
	this->host = host;
}

void VirtualServer::setPort(std::string const& port)
{
	this->port = port;
}

void VirtualServer::setFd(int const& fd)
{
	this->fd = fd;
}
