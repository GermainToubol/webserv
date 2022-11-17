// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/17 12:53:57 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/**
 * @file configure.cpp
 */

#include <fstream>
#include <iostream>
#include <new>
#include <string>
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
	if (count >= 0 && count < 8192)
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
}
