/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configure.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:42:29 by gtoubol           #+#    #+#             */
//   Updated: 2022/11/25 20:39:27 by gtoubol          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

/** @file configure.hpp */

#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP
#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "ConfigEntry.hpp"
#include "ConfigTree.hpp"
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
	std::vector<VirtualServer>							const& getServers(void) const;
	std::map<std::string, std::vector<VirtualServer*> >	const& getDuoIVS(void) const;

private:
	int		readFile(void);
	bool	readLine(std::string &);
	void	parse(std::string const&);
	void	addServer(ConfigEntry const&);
	void	addServerName(ConfigEntry const&);
	void	addRoot(ConfigEntry const&);
	void	parseError(std::string const&);
	void	putError(std::string const&);
	void	addEntryToTree(ConfigEntry const&);
	void	TreeToServers(void);
	void	setServerProperties(ConfigTree const&, VirtualServer&);

	// List of all properties addition to a single VirtualServer
	void	addListen(ConfigTree const&, VirtualServer&);
	bool	setPort(std::string const&, VirtualServer&);
	void	setHost(std::string const&, VirtualServer&);
	bool	validHost(std::string const&, VirtualServer&);

	std::string											filename;
	std::ifstream										_ifs;
	int													_status;
	std::vector<VirtualServer>							server_list;
	std::map<std::string, std::vector<VirtualServer*> >	duoIVS;
	size_t												n_line;
	ConfigTree											*tree;
};

#endif
