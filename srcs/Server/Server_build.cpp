/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_build.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 20:26:43 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 21:50:40 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstring>

#include <sys/epoll.h>

#include "WebServer.hpp"

int	WebServer::openFile(Setup *setup, Response *response)
{
	Cache			cache;
	std::ifstream	*stream;

	for (std::vector<Cache>::iterator it = this->_all_cache.begin(); it != this->_all_cache.end(); it++)
	{
		if (it->getUri() == response->getFilename())
		{
			it->setUsers(it->getUsers() + 1);
			response->setBodySize(it->getSize());
			return (0);
		}
	}
	try {stream = new std::ifstream(response->getFilename().c_str());}
	catch (std::exception &e) {return (perror("/!\\ Open file failed"), 1);}

	stream->seekg(0, stream->end);
	cache.setSize(stream->tellg());
	stream->seekg(0, stream->beg);
	cache.setStream(stream);
	cache.setUri(response->getFilename());
	cache.setUsers(1);
	response->setBodySize(cache.getSize());
	
	this->_all_cache.push_back(cache);
	
	return (0);
}

int WebServer::buildResponseDefault(int client_fd, Request *request, Setup *setup)
{
	struct epoll_event	event;
	Response			response;
	std::string			status;

	std::cerr << "[ Build response default ]" << std::endl;

	response.setFd(client_fd);
	response.setStatus(0);
	response.setPosition(0);
	if (setup->getServer() == 0)
	{
		if (this->_status_codes.find(setup->getCode()) != this->_status_codes.end()) //set dans le if apres
			status = this->_status_codes.find(setup->getCode())->second;
		else
			status = "Unknown";
		setup->setExtension(".html");
		response.setBody(setup->getCode(), status);
	}
	else
	{
		setup->setUri(setup->getCode());
		if (setup->getUri() == "")
			return (setup->setServer(0), buildResponseDefault(client_fd, request, setup));
		setup->setExtension();
		if (request->getLocation()->getCgiPerm().find(setup->getExtension()) != request->getLocation()->getCgiPerm().end())
			return (this->cgiMode(request, setup));
		if (this->openFile(setup, &response)) 
			return (setup->setServer(0), buildResponseDefault(client_fd, request, setup));
		response.setFilename(setup->getUri());
	}
	response.setHeader(setup, this->_status_codes, this->_mimetypes, response.getBodySize());
	this->_all_response.push_back(response);

	std::memset(&event, 0, sizeof(event));
	event.data.fd = client_fd;
 	event.events = EPOLLOUT;
 	epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, client_fd, &event);
	return (0);
}

int	WebServer::buildResponseListing(Request *request, Setup *setup, int client_fd)
{
	struct epoll_event	event;
	Response			response;

	std::cerr << "[ Build response listing ]" << std::endl;

	response.setFd(client_fd);
	response.setStatus(0);
	response.setPosition(0);
	if (response.setListingBody(setup->getUri(), setup->getServer()->getRoot()))
		return (setup->setCode(500), 500);

	setup->setExtension(".html");
	response.setHeader(setup, this->_status_codes, this->_mimetypes, response.getBodySize());
	
	this->_all_response.push_back(response);

	std::memset(&event, 0, sizeof(event));
	event.data.fd = client_fd;
 	event.events = EPOLLOUT;
 	epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, client_fd, &event);
	return (0);
}