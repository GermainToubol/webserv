/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:03:52 by gtoubol           #+#    #+#             */
/*   Updated: 2022/11/23 11:40:48 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "VirtualServer.hpp"

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
	return (server_name);
}

std::string const& VirtualServer::getRoot() const
{
	return (root);
}

std::string const& VirtualServer::getHost() const
{
	return (host);
}

std::string const& VirtualServer::getPort() const
{
	return (port);
}

int const& VirtualServer::getFd() const
{
	return (fd);
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
