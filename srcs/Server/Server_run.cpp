/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_run.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:53:55 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/24 21:40:27 by lgiband          ###   ########.fr       */
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

int WebServer::buildResponse(int client_fd, Request *request, Setup const &setup)
{
	struct epoll_event	event;
	(void)client_fd;
	(void)request;
	(void)setup;

	std::memset(&event, 0, sizeof(event));
	event.data.fd = client_fd;
 	event.events = EPOLLOUT;
 	epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, client_fd, &event);
	return (0);
}

int	WebServer::setResponse(int client_fd, Request *request)
{
	int					ret;
	Setup				setup;

	ret = request->parsing(&setup);
	if (ret != 0)
		return (this->buildResponse(client_fd, request, setup));
	ret = request->getServer(&setup, this->getAccessibleServer(client_fd));
	if (ret != 0)
		return (this->buildResponse(client_fd, request, setup));
	ret = request->getLocation(&setup);
	if (ret != 0)
		return (this->buildResponse(client_fd, request, setup));
	ret = request->setUri(&setup);
	if (ret != 0)
		return (this->buildResponse(client_fd, request, setup));
	//check error
	
	return(request->modeChoice(&setup, client_fd));

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
		// this->add_request(client_fd, ret);
		// if (std::strstr(this->get_request(events[i].data.fd, 0).c_str(), "\r\n\r\n") || ret != BUFFER_SIZE)
		// {
		// 	std::cout << "All request receive" << std::endl;
		// 	this->set_response(events[i].data.fd);

		// }
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