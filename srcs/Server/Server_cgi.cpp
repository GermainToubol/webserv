/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 15:18:26 by fmauguin          #+#    #+#             */
/*   Updated: 2022/12/01 16:03:24 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>

#include <sys/socket.h>
#include <unistd.h>

#include "WebServer.hpp"

bool	WebServer::isCgi(int file_fd)
{
	if (this->_cgiFD.find(file_fd) != this->_cgiFD.end())
		return (true);
	return (false);
}

int	WebServer::cgiResponse(int file_fd)
{
	int	client_fd;
	int	readed;
	int	sended;
	Response *response;
	std::string	buf;
	std::string::size_type	end;

	client_fd = this->_cgiFD.find(file_fd)->second;
	response = this->getResponse(client_fd);

	if (response->getStatus() == 0 || response->getHeader() == "")
	{
		sended = send(client_fd, response->getHeader().c_str(), std::min((size_t)SEND_SIZE, response->getHeader().size()), MSG_NOSIGNAL | MSG_MORE);
		if (sended == -1)
		{
			this->removeResponse(client_fd);
			close(file_fd);
			return (0);
		}
		if (sended >= response->getHeader().size())
		{
			response->setStatus(1);
			response->setHeader("");
		}
		else
			response->eraseHeader(0, sended);
	}
	if (response->getStatus() == 1)
	{
		readed = recv(file_fd, this->_buffer, BUFFER_SIZE, MSG_NOSIGNAL);
		if (readed == 0 || readed == -1)
		{
			this->removeResponse(client_fd);
			close(file_fd);
			return (0);
		}
		buf = std::string(this->_buffer, readed);
		if ((end = buf.find("\n\n") != std::string::npos) || (end = buf.find("\r\n\r\n") != std::string::npos))
			response->setHeader(response->getHeader() + buf.substr(0, end));
	}

}
