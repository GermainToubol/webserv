/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_core.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:19:07 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 13:54:45 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <iostream>

#include "WebServer.hpp"
#include "Configure.hpp"

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