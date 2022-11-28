/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_modeChoice.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:44:45 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/28 15:34:43 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstring>

#include <sys/epoll.h>

#include "WebServer.hpp"
#include "utils.hpp"

int	WebServer::redirectMode(Request *request, Setup *setup, int client_fd)
{
	struct epoll_event	event;
	Response			response;

	std::cerr << "[ Build response Redirect ]" << std::endl;
	setup->setCode(301);
	if (request->getLocation()->getRedirect()[0] == '/')
		setup->addField("Location", request->getLocation()->getRedirect());
	else
		setup->addField("Location", "/" + request->getLocation()->getRedirect());
	std::cerr << "[ Redirect to " << request->getLocation()->getRedirect() << " ]" << std::endl;
	response.setFd(client_fd);
	response.setStatus(0);
	response.setPosition(0);
	response.setHeader(setup, this->_status_codes, this->_mimetypes, 0);

	std::cerr << "[ Header builded ]\n" << response.getHeader() << "[ End Header ]" << std::endl;

	this->_all_response.push_back(response);

	std::memset(&event, 0, sizeof(event));
	event.data.fd = client_fd;
 	event.events = EPOLLOUT;
 	epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, client_fd, &event);
	return (0);
}

int	WebServer::cgiMode(Request *request, Setup *setup, int client_fd)
{
	(void)setup;
	(void)request;
	(void)client_fd;
	return (0);
}

int	WebServer::getMode(Request *request, Setup *setup, int client_fd)
{
	std::cerr << "[ Get Mode ]" << std::endl;

	std::cerr << "[ Uri : " << setup->getUri() << " ]" << std::endl;
	if (!this->doesPathExist(setup->getUri()))
		return (setup->setCode(404), 404);
	if (!this->isPathReadable(setup->getUri()))
		return (setup->setCode(403), 403);
	if (this->isDirectory(setup->getUri()) && request->getLocation()->getAutoindex() == false)
	{
		if (request->getLocation()->getDefaultFile() == "" || request->getLocation()->getDefaultFile() == setup->getUri())
			return (setup->setCode(404), 404);
		else
		{
			// ca va probablement pas marcher parce que le file va dependre de la root de location et de la root generale
			// peut etre que je peux recuperer le path de la root de location dans l'uri, erase la fin et ajouter le default file
			setup->addUri(request->getLocation()->getDefaultFile());
			setup->setExtension();
			if (request->getLocation()->getCgiPerm().find(setup->getExtension()) != request->getLocation()->getCgiPerm().end())
				return (this->cgiMode(request, setup, client_fd));
			else
				return (this->getMode(request, setup, client_fd));
		}
	}
	if (this->isDirectory(setup->getUri()) && request->getLocation()->getAutoindex() == true)
		return (setup->setCode(200), this->buildResponseListing(request, setup, client_fd));
	if (this->isFile(setup->getUri()))
		return (setup->setCode(200), this->buildResponseGet(request, setup, client_fd));
	return (setup->setCode(403), 403);
}

int	WebServer::postMode(Request *request, Setup *setup, int client_fd)
{
	(void)request;
	(void)setup;
	(void)client_fd;
	return (derror("/!\\ Not Implemented"), setup->setCode(501), 501);
}

int	WebServer::deleteMode(Request *request, Setup *setup, int client_fd)
{
	(void)request;
	(void)setup;
	(void)client_fd;
	return (derror("/!\\ Not Implemented"), setup->setCode(501), 501);
}

int	WebServer::modeChoice(Request *request, Setup *setup, int client_fd)
{
	std::cerr << "[ Mode choice ]" << std::endl;
	
	if (request->getLocation()->getRedirect() != "" && !isMe(setup->getUri(), request->getLocation()->getRedirect(), setup->getServer()->getRoot()))
		return (this->redirectMode(request, setup, client_fd));
	
	if (request->getMethod() == "GET" && !(request->getLocation()->getPermission() & GET_PERM))
		return (derror("/!\\ GET not allowed"), setup->setCode(405), 405);
	if (request->getMethod() == "POST" && !(request->getLocation()->getPermission() & POST_PERM))
		return (derror("/!\\ POST not allowed"), setup->setCode(405), 405);
	if (request->getMethod() == "DELETE" && !(request->getLocation()->getPermission() & DEL_PERM))
		return (derror("/!\\ DELETE not allowed"), setup->setCode(405), 405);
	
	if (request->getLocation()->getCgiPerm().find(setup->getExtension()) != request->getLocation()->getCgiPerm().end())
		return (this->cgiMode(request, setup, client_fd));
	
	if (request->getMethod() == "GET")
		return (this->getMode(request, setup, client_fd));
	if (request->getMethod() == "POST")
		return (this->postMode(request, setup, client_fd));
	if (request->getMethod() == "DELETE")
		return (this->deleteMode(request, setup, client_fd));
	return (derror("/!\\ Not Implemented"), setup->setCode(501), 501);
}
