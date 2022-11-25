/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:48:17 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 13:49:43 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(): _filename(""), _header(""), _body(""), _body_size(0), _position(0), _fd(-1), _status(0) {}

Response::~Response() {}

/*Accesseurs*/
std::string const&	Response::getFilename() const
{
	return (this->_filename);
}

std::string const&	Response::getHeader() const
{
	return (this->_header);
}

std::string const&	Response::getBody() const
{
	return (this->_body);
}

std::string::size_type const&	Response::getBodySize() const
{
	return (this->_body_size);
}

std::string::size_type const&	Response::getPosition() const
{
	return (this->_position);
}

int	const&	Response::getFd() const
{
	return (this->_fd);
}

int	const&	Response::getStatus() const
{
	return (this->_status);
}

void	Response::setFilename(std::string const& filename)
{
	this->_filename = filename;
}

void	Response::setHeader(std::string const& header)
{
	this->_header = header;
}

void	Response::setBody(std::string const& body)
{
	this->_body = body;
}

void	Response::setBodySize(std::string::size_type const& body_size)
{
	this->_body_size = body_size;
}

void	Response::setPosition(std::string::size_type const& position)
{
	this->_position = position;
}

void	Response::setFd(int const& fd)
{
	this->_fd = fd;
}

void	Response::setStatus(int const& status)
{
	this->_status = status;
}
