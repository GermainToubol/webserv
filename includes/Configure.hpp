/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configure.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:42:29 by gtoubol           #+#    #+#             */
//   Updated: 2022/11/28 12:01:26 by gtoubol          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

/** @file configure.hpp */

#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP
#pragma once

#include <cstddef>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "ConfigEntry.hpp"
#include "ConfigTree.hpp"
#include "VirtualServer.hpp"

// number of available ports
#define MAX_PORT_NBR 65536

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
	void	parseError(std::string const&);
	void	putError(std::string const&, size_t);
	void	addEntryToTree(ConfigEntry const&);
	void	TreeToServers(void);
	void	setServerProperties(ConfigTree const&, VirtualServer&);

	// List of all properties addition to a single VirtualServer
	void	addListen(ConfigTree const&, VirtualServer&);
	bool	setPort(std::string const&, VirtualServer&, size_t);
	void	setHost(std::string const&, VirtualServer&, size_t);
	bool	validHost(std::string const&, VirtualServer&, size_t);

	template<class T>
	void	addRoot(ConfigTree const&, T&);
	template<class T>
	void	addIndex(ConfigTree const&, T&);
	template<class T>
	void	addPermission(ConfigTree const&, T&);

	void	addServerName(ConfigTree const&, VirtualServer&);
	void	addLocation(ConfigTree const&, VirtualServer&);
	void	setLocation(ConfigTree const&, Location&);

	std::string											filename;
	std::ifstream										_ifs;
	int													_status;
	std::vector<VirtualServer>							server_list;
	std::map<std::string, std::vector<VirtualServer*> >	duoIVS;
	size_t												n_line;
	ConfigTree											*tree;
};

#endif
