/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_core.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:19:07 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 20:51:17 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <iostream>

#include "WebServer.hpp"
#include "Configure.hpp"
#include "VirtualServer.hpp"

#define DEBUG 0

WebServer::WebServer(Configure const& config)
{
	std::multimap<std::string, std::string>	*multimap;
	std::map<int, std::string>				*map;

	this->_virtual_servers = config.getServers();
	multimap = &this->_mimetypes;
	#include "mime_types"
	map = &this->_status_codes;
	#include "status_codes"
	
	if (DEBUG)
	{
		for (std::multimap<std::string, std::string>::iterator it = multimap->begin(); it != multimap->end(); it++)
			std::cerr << it->first << ":" << it->second << std::endl;
		for (std::map<int, std::string>::iterator it = map->begin(); it != map->end(); it++)
			std::cerr << it->first << ":" << it->second << std::endl;
	}
}

WebServer::~WebServer(void)
{}

std::vector<VirtualServer> const& WebServer::getVirtualServers() const
{
	return (this->_virtual_servers);
}

std::multimap<std::string, std::string> const& WebServer::getMimeTypes() const
{
	return (this->_mimetypes);
}

VirtualServer const& WebServer::getEntryServer(int fd) const
{
	int fd = this->_duoCS.find(fd)->second;
	
	for (std::vector<VirtualServer>::const_iterator it = this->_virtual_servers.begin(); it != this->_virtual_servers.end(); it++)
	{
		if (it->getFd() == fd)
			return (*it);
	}
	return (*this->_virtual_servers.begin());
}