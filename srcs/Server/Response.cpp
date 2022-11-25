/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:48:17 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 21:54:32 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>

#include "Response.hpp"
#include "utils.hpp"

Response::Response(): _filename(""), _header(""), _body(""), _body_size(0), _position(0), _fd(-1), _send_status(0) {}

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
	return (this->_send_status);
}

void	Response::setFilename(std::string const& filename)
{
	this->_filename = filename;
}

void	Response::setHeader(std::string const& header)
{
	this->_header = header;
}

void	Response::setHeader(Setup *setup, std::map<int, std::string> const& status_code, std::multimap<std::string, std::string> const& mime_type, size_t body_size)
{
	std::string	type;
	std::string	status;

	if (mime_type.find(setup->getExtension()) != mime_type.end() && body_size > 0)
		type = mime_type.find(setup->getExtension())->second;
	else if (body_size > 0)
		type = "text/plain";
	if (status_code.find(setup->getCode()) != status_code.end())
		status = status_code.find(setup->getCode())->second;
	else
		status = "Unknown";

	this->_header = "HTTP/1.1 " + to_string(setup->getCode()) + " " + status + "\r\n";
	this->_header += "Content-Length: " + to_string(body_size) + "\r\n";
	if (body_size > 0)
		this->_header += "Content-Type: " + type + "\r\n";
	this->_header += "Connection: close\r\n";
	this->_header += setup->getFields();
	this->_header += "\r\n";
}

void	Response::setBody(std::string const& body)
{
	this->_body = body;
	this->_body_size = this->_body.size();
}

void	Response::setBody(int code, std::string const& type)
{
	this->_body = "\
<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"utf-8\">\
<title>Error " + to_string(code) + "</title>\
</head>\
<body>\
	<h1>Error " + to_string(code) + ": " + type + "</h1>\
</body>\
</html>";
	this->_body_size = this->_body.size();
}

int	Response::setListingBody(std::string uri, std::string const& root)
{
	DIR				*dir;
	struct dirent	*ent;

	uri.replace(0, root.size(), "");
	if (uri[0] != '/')
		uri.insert(0, "/");
	if (*(uri.end() - 1) != '/')
		uri += '/';
	this->_body = "<!DOCTYPE html>\n\
<html>\n<head>\n<title>Index of " + uri + "</title>\n</head>\n\
<body>\n<h1 style=\"font-size:30px\">Index of " + uri + "</h1>\n<br><br><hr>\n";
	dir = opendir(uri.c_str());
	if (dir == NULL)
		return (500);
	while ((ent = readdir(dir)) != NULL)
		this->_body += "<a style=\"margin: 5px; font-size: 20px; font-style: italic;\" href=\"" + uri + ent->d_name + "\">" + ent->d_name + "</a><br><hr>\n";
	this->_body += "</body>\n</html>";
	closedir(dir);
	this->_body_size = this->_body.size();
	return (0);
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
	this->_send_status = status;
}
