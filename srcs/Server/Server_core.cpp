/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_core.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:19:07 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/28 10:45:18 by lgiband          ###   ########.fr       */
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
	
	this->_duoIVS = config.getDuoIVS();
	std::cerr << " [ duoIVS (server side) size: " << this->_duoIVS.size() << " ]" << std::endl;
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

Response *WebServer::getResponse(int client_fd) const
{
	for (std::vector<Response>::const_iterator it = this->_all_response.begin(); it != this->_all_response.end(); it++)
	{
		if (it->getFd() == client_fd)
			return ((Response*)&(*it));
	}
	return (NULL);
}

Cache	*WebServer::getCache(std::string const& filename) const
{
	for (std::vector<Cache>::const_iterator it = this->_all_cache.begin(); it != this->_all_cache.end(); it++)
	{
		if (it->getUri() == filename)
			return ((Cache*)&(*it));
	}
	return (NULL);
}

std::vector<VirtualServer*> const* WebServer::getAccessibleServer(int client_fd) const
{
	if (this->_duoCS.find(client_fd) == this->_duoCS.end())
		return (NULL);
	int server_fd = this->_duoCS.find(client_fd)->second;
	if (this->_duoSI.find(server_fd) == this->_duoSI.end())
		return (NULL);
	std::string interface = this->_duoSI.find(server_fd)->second;
	if (this->_duoIVS.find(interface) == this->_duoIVS.end())
		return (NULL);
	std::vector<VirtualServer*> const * servers = &(this->_duoIVS.find(interface)->second);
	return (servers);
}