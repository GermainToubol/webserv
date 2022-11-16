// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/16 12:53:47 by gtoubol          ###   ########.fr       //
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

Configure::Configure(std::string const& file): filename(file), _ifs(), _status(1)
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
	char	buffer[4096];
	ssize_t	count;

	_ifs.getline(buffer, 4096);
	count = _ifs.gcount();
	if (count > 0 && (size_t)count < (current_line.capacity() - current_line.size()))
	{
		current_line.append(buffer);
		if (buffer[count] == '\n')
		{
			this->parse(current_line);
			for (size_t i = 0; i < 4096; ++i)
			{
				current_line[i] = '\0';
			}
		}
	}
	else if (count == 0)
	{
		this->parse(current_line);
	}
	else
	{
		std::cout << "coucou\n";
	}
	return (_ifs.good());
}


void	Configure::parse(std::string const& line)
{
	(void)line;
}
