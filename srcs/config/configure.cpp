// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/18 14:58:27 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/**
 * @file configure.cpp
 */

#include <algorithm>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <new>
#include <string>
#include <sys/socket.h>
#include "configure.hpp"
#include "ConfigEntry.hpp"

#define MAX_LINE_SIZE 8192

Configure::Configure(std::string const& file):
	filename(file),
	_ifs(),
	_status(1),
	server_list(),
	n_line(0)
{
	_ifs.open(filename.c_str());
	if (_ifs.good())
	{
		_status = 0;
		this->readFile();
	}
	else
	{
		std::cerr << "Error: " << filename << ": cannot read the file." << std::endl;
		_status = 1;
	}
	_ifs.close();
}

int Configure::isGood(void) const
{
	return (_status == 0);
}

/**
 * @brief Read the file and wrap the lexer/parser
 *
 * @return
 * @exception
 */
int Configure::readFile(void)
{
	std::string current_line;

	try
	{
		// Allow sufficient amount of memory at one
		current_line.reserve(8192);
	} catch (std::bad_alloc const& e)
	{
		_status = 128;
		std::cerr << "Error: memory allocation error.\n";
		return (-1);
	}
	while (this->readLine(current_line));
	if (_ifs.fail() && _ifs.bad())
	{
		_status = 1;
		std::cerr << "Error: " << this->filename << ": unreadable.\n";
	}
	return (0);
}

/**
 * @fix fin de ficher != \n
 *
 */
bool	Configure::readLine(std::string &current_line)
{
	char	buffer[8192];
	ssize_t	count;

	_ifs.getline(buffer, 8192);
	count = _ifs.gcount();
	if (count >= 0 and count < 8192)
	{
		++this->n_line;
		current_line.append(buffer);
		this->parse(current_line);
		current_line.clear(); // clear content but not allocated memory
	}
	return (_ifs.good());
}

void	Configure::parse(std::string const& line)
{
	ConfigEntry entry(line);

	std::cout << entry.getLevel() << ">"
			  << entry.getKey() << ">"
			  << entry.getValue() << std::endl;
	if (entry.getKey() == "server")
	{
		this->addServer(entry);
	}
	if (entry.getKey() == "listen")
	{
		this->addListen(entry);
	}
	if (entry.getKey() == "root")
	{
		this->addRoot(entry);
	}
}

void	Configure::addServer(ConfigEntry const& entry)
{
	if (entry.getLevel() != 0)
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": server level needs to be 0" << std::endl;
		_status = 1;
		return ;
	}
	if (!entry.hasDelimiter())
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": missing delimiter" << std::endl;
		_status = 1;
		return ;
	}
	if (!entry.isValueEmpty())
	{
		std::cerr << "Bad config: line " << this->n_line
				 << ": configuration file error" << std::endl;
		_status = 1;
		return ;
	}
	this->server_list.push_back(VirtualServer());
}

void	Configure::addListen(ConfigEntry const& entry)
{
	std::string port = "";
	std::string address = "";
	std::string::const_reverse_iterator rit;

	if (this->server_list.size() == 0)
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": listen block should be in server block" << std::endl;
		_status = 1;
		return ;
	}
	if (entry.getLevel() != 2)
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": listen level needs to be 1" << std::endl;
		_status = 1;
		return ;
	}
	if (!entry.hasDelimiter())
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": missing delimiter" << std::endl;
		_status = 1;
		return ;
	}
	if (this->server_list.back().getPort() != "")
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": server blocks have only one listen" << std::endl;
		_status = 1;
		return ;
	}
	rit = entry.getValue().rbegin();
	while (rit !=  entry.getValue().rend() and isspace(*rit))
	{
		++rit;
	}
	while (rit != entry.getValue().rend() and isdigit(*rit))
	{
		port.push_back(*rit);
		++rit;
	}
	if (rit != entry.getValue().rend() and *rit == ':')
	{
		++rit;
		while (rit != entry.getValue().rend() and (*rit == '.' or isdigit(*rit)))
		{
			address.push_back(*rit);
			++rit;
		}
	}
	while (rit != entry.getValue().rend() and isspace(*rit))
	{
		++rit;
	}
	if (rit != entry.getValue().rend() or port == "")
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": listen block values must match the format [IPV4:]PORT" << std::endl;
		_status = 1;
		return ;
	}
	std::reverse(port.begin(), port.end());
	if (address != "")
	{
		std::reverse(address.begin(), address.end());
		if (!this->validHost(address))
		{
			std::cerr << "Bad config: line " << this->n_line
					  << ": listen block values must match the format [IPV4:]PORT" << std::endl;
			_status = 1;
			return ;
		}
		this->server_list.back().setHost(address);
	}
	this->server_list.back().setPort(port);
}

bool Configure::validHost(std::string const& address)
{
	in_addr_t	ip;

	ip = inet_addr(address.c_str());
	if (ip == 0xffffffff && address != "255.255.255.255")
	{
		return (false);
	}
	return (true);
}

void	Configure::addRoot(ConfigEntry const& entry)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;

	if (this->server_list.size() == 0)
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": root block should be in server block" << std::endl;
		_status = 1;
		return ;
	}
	if (entry.getLevel() != 2)
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": root level needs to be 1" << std::endl;
		_status = 1;
		return ;
	}
	if (!entry.hasDelimiter())
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": missing delimiter" << std::endl;
		_status = 1;
		return ;
	}
	if (this->server_list.back().getRoot() != "")
	{
		std::cerr << "Bad config: line " << this->n_line
				  << ": server blocks have only one root" << std::endl;
		_status = 1;
		return ;
	}
	for (it = entry.getValue().begin(); it != entry.getValue().end(); ++it)
	{
		if (not isspace(*it))
			break ;
	}
	for (rit = entry.getValue().rbegin(); rit != entry.getValue().rend(); ++rit)
	{
		if (not isspace(*rit))
			break ;
	}
	std::string root;
	root.assign(it, rit.base() + 1);
	this->server_list.back().setRoot(root);
}
