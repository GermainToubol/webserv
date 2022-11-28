/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configure.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             */
//   Updated: 2022/11/26 00:04:34 by gtoubol          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */


/**
 * @file configure.cpp
 */

#include <algorithm>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <new>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include "Configure.hpp"
#include "ConfigEntry.hpp"
#include "ConfigTree.hpp"


#define MAX_LINE_SIZE 8192

Configure::Configure(std::string const& file):
	filename(file),
	_ifs(),
	_status(1),
	server_list(),
	n_line(0)
{
	ConfigTree parse_tree;
	this->tree = &parse_tree;
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
	this->tree->print("");
	this->tree = NULL;
}

std::vector<VirtualServer> const& Configure::getServers(void) const
{
	return (this->server_list);
}

/**
 * @brief Extract the list of virtual servers related to their full interface description
 *
 * The string describes the interface as "IPV4:IP"
 */
std::map<std::string, std::vector<VirtualServer*> > const& Configure::getDuoIVS(void) const
{
	return (this->duoIVS);
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

	current_line.reserve(8192);
	while (this->readLine(current_line))
	{
		;
	}
	if (_ifs.fail() && _ifs.bad())
	{
		_status = 1;
		std::cerr << "Error: " << this->filename << ": unreadable.\n";
	}
	if (this->isGood())
	{
		this->TreeToServers();
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
		current_line.clear();
	}
	return (_ifs.good());
}

void	Configure::addEntryToTree(ConfigEntry const& entry)
{
	ConfigTree	*current_level;
	size_t		i;

	current_level = this->tree;
	for (i = 0; i < entry.getLevel() / 2; ++i)
	{
		if (current_level->getLeaves().empty())
			break;
		current_level = &(current_level->getLeaves().back());
	}
	if (entry.getKey() != "")
	{
		if (entry.getLevel() / 2 == i && entry.getLevel() % 2 == 0)
			current_level->getLeaves().push_back(ConfigTree(entry));
		else
			this->parseError("bad block level");
	}
	else if (entry.hasDelimiter() || (entry.getValue() != "" && entry.getValue()[0] != '#'))
	{
		this->parseError("invalid entry");
	}
}

void	Configure::parse(std::string const& line)
{
	ConfigEntry	entry(line);

	this->addEntryToTree(entry);
}

void	Configure::TreeToServers(void)
{
	// We go through each server and set each properties
	for (std::vector<ConfigTree>::const_iterator it_server = this->tree->getLeaves().begin();
		 it_server != this->tree->getLeaves().end(); ++it_server)
	{
		VirtualServer	current_server;
		if (it_server->getKey() != "server")
		{
			this->putError(it_server->getKey() + ": bad key level");
			continue ;
		}
		if (not it_server->hasDelimiter())
		{
			this->putError(it_server->getKey() + ": missing delimiter");
			continue ;
		}
		if (it_server->getValue() != "")
		{
			this->putError("server: unexpected value");
			continue ;
		}
		this->setServerProperties(*it_server, current_server);
		this->server_list.push_back(current_server);
	}
}

void	Configure::setServerProperties(ConfigTree const& node, VirtualServer& server)
{
	for (std::vector<ConfigTree>::const_iterator server_prop = node.getLeaves().begin();
		 server_prop != node.getLeaves().end();
		 ++server_prop
		)
	{
		if (server_prop->getKey() == "listen")
		{
			this->addListen(*server_prop, server);
		}
		if (server_prop->getKey() == "root")
		{
			this->addRoot(*server_prop, server);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                         Listen-related properties                         //
///////////////////////////////////////////////////////////////////////////////
void	Configure::addListen(ConfigTree const& node, VirtualServer& server)
{
	std::string port = "";
	std::string host = "";
	if (not node.getLeaves().empty())
	{
		this->putError("listen block don't have son properties");
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("listen: bad format");
		return ;
	}
	if (not this->setPort(node.getValue(), server))
	{
		return ;
	}
	this->setHost(node.getValue(), server);
}

bool	Configure::setPort(std::string const& value, VirtualServer& server)
{
	std::string port_str;
	std::string::const_reverse_iterator rit;
	std::string::const_iterator it;
	long int port_nbr;
	char *end;

	// Skip all terminating spaces
	for (rit = value.rbegin(); rit != value.rend(); ++rit)
	{
		if (not isspace(*rit))
			break;
	}
	it = rit.base();
	for (; rit != value.rend(); ++rit)
	{
		if (not isdigit(*rit))
			break ;
	}
	port_str.assign(rit.base(), it);
	if (port_str != "")
	{
		port_nbr = strtol(port_str.c_str(), &end, 10);
		if (*end == '\0' && port_nbr >= 0 && port_nbr < 65536)
		{
			server.setPort(port_str);
			return (true);
		}
	}
	this->putError("listen: bad port format");
	return (false);
}

void Configure::setHost(std::string const& value, VirtualServer& server)
{
	std::string host_str;
	std::string::const_reverse_iterator rit;
	std::string::const_iterator it;

	for (rit = value.rbegin(); rit != value.rend(); ++rit)
	{
		if (not isspace(*rit))
			break ;
	}
	for (; rit != value.rend(); ++rit)
	{
		if (not isdigit(*rit))
			break;
	}
	for (it = value.begin(); it != rit.base(); ++it)
	{
		if (not isspace(*it))
			break ;
	}
	if (*rit != ':' && rit.base() != it)
	{
		this->putError("listen: bad format");
		return ;
	}
	else if (*rit == ':')
		++rit;
	host_str.assign(it, rit.base());
	if (host_str == "")
	{
		host_str = "0.0.0.0";
	}
	if (this->validHost(host_str, server))
	{
		server.setHost(host_str);
	}
}

bool Configure::validHost(std::string const& address, VirtualServer& server)
{
	const struct addrinfo hint = {
		AI_PASSIVE | AI_CANONNAME,
		AF_INET,
		SOCK_STREAM,
		0,
		0,
		NULL,
		NULL,
		NULL
	};
	struct addrinfo *res;
	char buffer[INET6_ADDRSTRLEN];

	(void)server;
	(void)hint;
	if (getaddrinfo(address.c_str(), server.getPort().c_str(), &hint, &res))
	{
		this->putError("listen: could not resolve `" + address + "`");
		return (false);
	}
	inet_ntop(res->ai_family, &((struct sockaddr_in *)res->ai_addr)->sin_addr, buffer, INET6_ADDRSTRLEN);
	freeaddrinfo(res);
	server.setHost(buffer);
	return (true);
}

///////////////////////////////////////////////////////////////////////////////
//                                    Root                                   //
///////////////////////////////////////////////////////////////////////////////
void	Configure::addRoot(ConfigTree const& node, VirtualServer &server)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;

	if (not node.getLeaves().empty())
	{
		this->putError("root block don't have son properties");
		return ;
	}
	if (!node.hasDelimiter())
	{
		this->putError("root: missing delimiter");
		return ;
	}
	for (it = node.getValue().begin(); it != node.getValue().end(); ++it)
	{
		if (not isspace(*it))
			break ;
	}
	for (rit = node.getValue().rbegin(); rit != node.getValue().rend(); ++rit)
	{
		if (not isspace(*rit))
			break ;
	}
	std::string root;
	root.assign(it, rit.base());
	if (root == "" or root[0] != '/')
	{
		return (this->putError("root: expect absolut path"));
	}
	server.setRoot(root);
}

///////////////////////////////////////////////////////////////////////////////
//                          Error-related functions                          //
///////////////////////////////////////////////////////////////////////////////
void	Configure::parseError(std::string const& msg)
{
	std::cerr << "Bad config: line " << this->n_line
			  << ": " << msg << std::endl;
	_status = 1;
}

void	Configure::putError(std::string const& msg)
{
	std::cerr << "Bad config: " << msg << std::endl;
	_status = 1;
}

void	Configure::addServerName(ConfigEntry const& entry)
{
	if (this->server_list.size() == 0)
	{
		return (this->parseError("server_name block should be in server block"));
	}
	if (entry.getLevel() / 2 != 1)
	{
		return (this->parseError("server_name level should be 1"));
	}
	if (!entry.hasDelimiter())
	{
		return (this->parseError("missing delimiter"));
	}
	if (this->server_list.back().getServerName() != "")
	{
		return (this->parseError("server blocks have only one server_name"));
	}
	this->server_list.back().setServerName(entry.getValue());
}
