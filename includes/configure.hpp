// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.hpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:42:29 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/16 12:53:02 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/** @file configure.hpp */

#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP
#pragma once

#include <fstream>
#include <string>

/**
 * @class Configure
 * @brief Tool to load and parse the configuration file.
 *
 * Wrapper around the lexer/parser of the configuration files (yaml style)
 */
class Configure
{
public:
	Configure(std::string const&);
	virtual ~Configure() {}

	int	isGood(void) const;

private:
	int		readFile(void);
	bool	readLine(std::string &);
	void	parse(std::string const&);

	std::string		filename;
	std::ifstream	_ifs;
	int				_status;
};

#endif
