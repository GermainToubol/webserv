// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.hpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:42:29 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/18 14:40:06 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/** @file configure.hpp */

#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP
#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "ConfigEntry.hpp"
#include "VirtualServer.hpp"

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
	std::vector<VirtualServer> const& getServers(void) const;

private:
	int		readFile(void);
	bool	readLine(std::string &);
	void	parse(std::string const&);
	void	addServer(ConfigEntry const&);
	void	addListen(ConfigEntry const&);
	void	addRoot(ConfigEntry const&);
	bool	validHost(std::string const&);

	std::string		filename;
	std::ifstream	_ifs;
	int				_status;
	std::vector<VirtualServer> server_list;
	size_t			n_line;
};

#endif
