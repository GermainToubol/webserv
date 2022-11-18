// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   VirtualServer.cpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/17 16:03:52 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/17 16:21:12 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

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

void VirtualServer::setServerName(std::string const& name)
{
	server_name = name;
}

void VirtualServer::setRoot(std::string const& directory)
{
	root = directory;
}

void VirtualServer::setHost(std::string const& host)
{
	this->host = host;
}

void VirtualServer::setPort(std::string const& port)
{
	this->port = port;
}
