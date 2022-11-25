/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:53:55 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 16:33:40 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "WebServer.hpp"
#include "Setup.hpp"

extern int running;

/*
* Take the connection from the server socket and add it to the epoll
*/
int	WebServer::new_connection(int server_fd)
{
	int					client_socket;
	struct epoll_event	event;

	std::cerr << "[ New connection on server " << server_fd << " ]" << std::endl;
	client_socket = accept(server_fd, NULL, NULL);
	if (client_socket == -1 && errno != EWOULDBLOCK)
		return (perror("/!\\ Accept failed"), -1);
	else
	{
		if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1)
			return (close(client_socket), perror("/!\\ Fcntl failed"), 1);
		
		std::memset(&event, 0, sizeof(event));
		event.data.fd = client_socket;
		event.events = EPOLLIN;
		epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, client_socket, &event);
		this->addDuoCS(client_socket, server_fd);
	}
	std::cout << "[ New client connected on " << client_socket << " ]" << std::endl;
	return (0);
}

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
		setup->setUri(setup->getCode()); // set avec location
		if (setup->getUri() == "") //pas de fichier d'erreur
			return (setup->setServer(0), buildResponseDefault(client_fd, request, setup));
		setup->setExtension(); //en recuperant l'uri
		if (request->getLocation()->getCgiPerm().find(setup->getExtension()) != request->getLocation()->getCgiPerm().end())
			return (this->cgiMode(request, setup));
		if (this->openFile(setup, &response)) //open failed //si path qui marche set dans le cache le fichier open avec pour nom setup->_uri //set body_size
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

int	WebServer::redirectMode(Request *request, Setup *setup, int client_fd)
{
	struct epoll_event	event;
	Response			response;

	std::cerr << "[ Build response default ]" << std::endl;
	setup->setCode(301);
	setup->addField("Location", request->getLocation()->getRedirect());
	response.setFd(client_fd);
	response.setStatus(0);
	response.setPosition(0);
	response.setHeader(setup, this->_status_codes, this->_mimetypes, 0);

	this->_all_response.push_back(response);

	std::memset(&event, 0, sizeof(event));
	event.data.fd = client_fd;
 	event.events = EPOLLOUT;
 	epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, client_fd, &event);
	return (0);
}

int	WebServer::cgiMode(Request *request, Setup *setup)
{
	(void)setup;
	return (0);
}

int	WebServer::modeChoice(Request *request, Setup *setup, int client_fd)
{
	if (request->getLocation()->getRedirect() != "")
		return (this->redirectMode(request, setup, client_fd));
	
	if (request->getMethod() == "GET" && !(request->getLocation()->getPermission() & GET_PERM))
		return (perror("/!\\ GET not allowed"), setup->setCode(405), 405);
	if (request->getMethod() == "POST" && !(request->getLocation()->getPermission() & POST_PERM))
		return (perror("/!\\ POST not allowed"), setup->setCode(405), 405);
	if (request->getMethod() == "DELETE" && !(request->getLocation()->getPermission() & DEL_PERM))
		return (perror("/!\\ DELETE not allowed"), setup->setCode(405), 405);
	
	if (request->getLocation()->getCgiPerm().find(setup->getExtension()) != request->getLocation()->getCgiPerm().end())
		return (this->cgiMode(request, setup));
	
	if (request->getMethod() == "GET")
		return (this->getMode(request, setup, client_fd));
	if (request->getMethod() == "POST")
		return (this->postMode(request, setup, client_fd));
	if (request->getMethod() == "DELETE")
		return (this->deleteMode(request, setup, client_fd));
	return (perror("/!\\ Method not allowed"), setup->setCode(405), 405);
}

int	WebServer::setResponse(int client_fd, Request *request)
{
	int					ret;
	Setup				setup;

	ret = request->parsing(&setup);
	if (ret != 0)
		return (this->buildResponseDefault(client_fd, request, &setup));
	ret = request->setServer(&setup, this->getAccessibleServer(client_fd));
	if (ret != 0)
		return (this->buildResponseDefault(client_fd, request, &setup));
	ret = request->setLocation(&setup);
	if (ret != 0)
		return (this->buildResponseDefault(client_fd, request, &setup));
	ret = request->setUri(&setup);
	if (ret != 0)
		return (this->buildResponseDefault(client_fd, request, &setup));
	ret = request->basicCheck(&setup);
	if (ret != 0)
		return (this->buildResponseDefault(client_fd, request, &setup));
	ret = this->modeChoice(request, &setup, client_fd);
	if (ret != 0)
		return (this->buildResponseDefault(client_fd, request, &setup));

	return (0);
}

/*
* Receive the request from the client and and stock it in the request vector
*/
int	WebServer::get_request(int client_fd)
{
	int		ret;
	int		state;
	Request	*request;
	
	ret = recv(client_fd, this->_buffer, BUFFER_SIZE, MSG_NOSIGNAL);
	if (ret == -1)
		perror("/!\\ Recv failed");
	else
	{
		this->_buffer[ret] = '\0';
		request = get_fd_request(client_fd);
		if (request == NULL)
			return (perror("/!\\ Request not found"), -1);
		state = request->addContent(this->_buffer);
		if (state == 1)
		{
			std::cerr << "[ All Request received on " << client_fd << " ]" << std::endl;
			this->setResponse(client_fd, request);
			remove_fd_request(client_fd);
		}
	}
	return (0);
}

int	WebServer::send_response(int client_fd)
{
	(void)client_fd;
	return (0);
}

int	WebServer::event_loop(struct epoll_event *events, int nb_events)
{
	int					listen_sock;

	for (int i = 0; i < nb_events; i++)
	{
		listen_sock = this->is_server(events[i].data.fd);
		if (listen_sock != -1)
			this->new_connection(listen_sock);
		else if (events[i].events & EPOLLIN)
			this->get_request(events[i].data.fd);
		else if (events[i].events & EPOLLOUT)
			this->send_response(events[i].data.fd);
		else
			epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
	}
	return (0);
}

int	WebServer::run(void)
{
	int					frame = 0;
	std::string			wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
	struct epoll_event	events[MAX_CLIENTS];
	int					nb_events;
	
	std::cerr << "\n=====================RUN=====================\n" << std::endl;
	
	while (running)
	{
		std::memset(events, 0, sizeof(events));
		nb_events = epoll_wait(this->_epoll_fd, events, MAX_CLIENTS, TIMEOUT);
		if (nb_events == -1)
			perror("Epoll_wait failed");
		else if (nb_events == 0)
		{
			std::cout  << "\33[2K\r" << wait[++frame % 6] << "\033[0;32m" << " Waiting for Connection... " << "\033[0m" << std::flush;
			if (frame >= 6)
				frame = 0;
			// this->clear_cache();
		}
		else
		{
			if (this->event_loop(events, nb_events) == -1)
				std::cout << "Error in requests" << std::endl;
		}
	}
	return (0);
}