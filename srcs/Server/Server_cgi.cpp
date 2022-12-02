/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 15:18:26 by fmauguin          #+#    #+#             */
/*   Updated: 2022/12/02 19:14:16 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <iostream>

#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>

#include "WebServer.hpp"
#include "utils.hpp"

extern int flags;

bool	WebServer::isCgi(int file_fd)
{
	if (this->_cgiFD.find(file_fd) != this->_cgiFD.end())
		return (true);
	return (false);
}

int	WebServer::closeCgiResponse(int client_fd, int file_fd)
{
	this->removeResponse(client_fd);
	close(file_fd);
	return (0);
}

int	WebServer::cgiResponse(int file_fd)
{
	int	client_fd;
	int	readed;
	int	sended;
	Response *response;
	std::string	buf;
	std::string::size_type	end;
	
	if (flags & FLAG_VERBOSE)
		std::cerr << "[ CGI Response ]" << std::endl;

	client_fd = this->_cgiFD.find(file_fd)->second;
	response = this->getResponse(client_fd);
	if (!response)
		return (this->closeCgiResponse(client_fd, file_fd));

	if (response->getStatus() == 0 || response->getStatus() == 2)
	{
		if (response->getHeader().find("Content-Length: ") != std::string::npos)
			response->setBodySize(std::strtol(response->getHeader().substr(response->getHeader().find("Content-Length: ") + 16).c_str(), NULL, 10));
		sended = send(client_fd, response->getHeader().c_str(), std::min((size_t)SEND_SIZE, response->getHeader().size()), MSG_NOSIGNAL | MSG_MORE);
		if (sended == -1)
			return (this->closeCgiResponse(client_fd, file_fd));
		
		if (flags & FLAG_VERBOSE)
			std::cerr << "[ Header CGI ]\n Header sended : " << response->getHeader() << " size: " << sended << std::endl;
		response->eraseHeader(0, sended);
		if (response->getHeader().size() == 0)
		{
			if (response->getStatus() == 0)
				response->setStatus(1);
			else if (response->getStatus() == 2)
				response->setStatus(3);
		}
	}
	else if (response->getStatus() == 1)
	{
		readed = read(file_fd, this->_buffer, BUFFER_SIZE);
		if (readed == 0 || readed == -1)
			return (this->closeCgiResponse(client_fd, file_fd));
		buf = std::string(this->_buffer, readed);
		if ((end = buf.find("\n\n") != std::string::npos) || (end = buf.find("\r\n\r\n") != std::string::npos))
		{
			response->addHeader(buf.substr(0, end + 2));
			response->setBodyAlone(buf.substr(end + 2));
			response->setStatus(2);
		}
		else
			response->addHeader(buf);
	}
	else if (response->getStatus() == 3)
	{
		if (response->getBody().size() > 0)
		{
			sended = send(client_fd, response->getBody().c_str(), std::min((size_t)SEND_SIZE, response->getBody().size()), MSG_NOSIGNAL);
			if (sended == -1)
				return (this->closeCgiResponse(client_fd, file_fd));
			if (flags & FLAG_VERBOSE)
				std::cerr << "[ CGI Response ] Body sended size: " << sended << std::endl;
			response->setPosition(response->getPosition() + sended);
			if (response->getPosition() >= response->getBodySize())
				return (this->closeCgiResponse(client_fd, file_fd));
			response->eraseBody(0, sended);
			if (response->getBody().size() == 0)
			{
				readed = read(file_fd, this->_buffer, BUFFER_SIZE);
				if (readed == 0 || readed == -1)
					return (this->closeCgiResponse(client_fd, file_fd));
				response->setBodyAlone(std::string(this->_buffer, readed));
			}
		}
	}
	return (0);
}
