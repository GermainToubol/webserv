/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configure.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             */
/*   Updated: 2022/11/30 13:54:40 by lgiband          ###   ########.fr       */
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
	duoIVS(),
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
	duoIVS.clear();
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
	ConfigEntry	entry(line, this->n_line);

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
			this->putError(it_server->getKey() + ": bad key level", it_server->getLineNumber());
			continue ;
		}
		if (not it_server->hasDelimiter())
		{
			this->putError(it_server->getKey() + ": missing delimiter", it_server->getLineNumber());
			continue ;
		}
		if (it_server->getValue() != "")
		{
			this->putError("server: unexpected value", it_server->getLineNumber());
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
			continue ;
		}
		if (server_prop->getKey() == "root")
		{
			this->addRoot(*server_prop, server);
			continue ;
		}
		if (server_prop->getKey() == "server_name")
		{
			this->addServerName(*server_prop, server);
			continue ;
		}
		if (server_prop->getKey() == "location")
		{
			this->addLocation(*server_prop, server);
			continue ;
		}
		this->putError(server_prop->getKey() + ": unknown property", node.getLineNumber());
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
		this->putError("listen: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("listen: bad format", node.getLineNumber());
		return ;
	}
	if (not this->setPort(node.getValue(), server, node.getLineNumber()))
	{
		return ;
	}
	this->setHost(node.getValue(), server, node.getLineNumber());
}

bool	Configure::setPort(std::string const& value, VirtualServer& server, size_t line_nb)
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
		if (*end == '\0' and port_nbr >= 0 and port_nbr < MAX_PORT_NBR)
		{
			server.setPort(port_str);
			return (true);
		}
	}
	this->putError("listen: bad port format", line_nb);
	return (false);
}

void Configure::setHost(std::string const& value, VirtualServer& server, size_t line_nb)
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
		this->putError("listen: bad format", line_nb);
		return ;
	}
	else if (*rit == ':')
		++rit;
	host_str.assign(it, rit.base());
	if (host_str == "")
	{
		host_str = "0.0.0.0";
	}
	if (this->validHost(host_str, server, line_nb))
	{
		server.setHost(host_str);
	}
}

bool Configure::validHost(std::string const& address, VirtualServer& server, size_t line_nb)
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
		this->putError("listen: could not resolve `" + address + "`", line_nb);
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
template<class T>
void	Configure::addRoot(ConfigTree const& node, T &server)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string root;

	if (not node.getLeaves().empty())
	{
		this->putError("root: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("root: missing delimiter", node.getLineNumber());
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
	root.assign(it, rit.base());
	if (root == "" or root[0] != '/')
	{
		return (this->putError("root: expect absolut path", node.getLineNumber()));
	}
	server.setRoot(root);
}

///////////////////////////////////////////////////////////////////////////////
//                                Server_name                                //
///////////////////////////////////////////////////////////////////////////////
// For now: only one name per server-block

void	Configure::addServerName(ConfigTree const& node, VirtualServer& server)
{
	std::string::const_iterator it;
	// std::string::const_reverse_iterator rit;
	std::string server_name;

	if (not node.getLeaves().empty())
	{
		this->putError("server_name: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("server_name: missing delimiter", node.getLineNumber());
		return ;
	}
	for (it = node.getValue().begin(); it != node.getValue().end(); ++it)
	{
		if (not isspace(*it))
			break ;
	}
	for (; it != node.getValue().end(); ++it)
	{
		if (isalnum(*it) or (*it == '.') or (*it == '-') or (*it == '_'))
			server_name.push_back(tolower(*it));
		else
			break ;
	}
	for (; it != node.getValue().end(); ++it)
	{
		if (not isspace(*it))
		{
			this->putError("server_name: invalid character", node.getLineNumber());
			return ;
		}
	}
	if (server_name == "")
	{
		this->putError("server_name: invalid value", node.getLineNumber());
		return ;
	}
	server.setServerName(server_name);
}

///////////////////////////////////////////////////////////////////////////////
//                                  Location                                 //
///////////////////////////////////////////////////////////////////////////////
void	Configure::addLocation(ConfigTree const& node, VirtualServer& server)
{
	std::string::const_iterator it;
	std::string value;
	Location	location(server);

	if (not node.hasDelimiter())
	{
		this->putError("location: missing delimiter", node.getLineNumber());
		return ;
	}
	for (it = node.getValue().begin(); it != node.getValue().end(); ++it)
	{
		if (not isspace(*it))
			break ;
	}
	for (; it != node.getValue().end(); ++it)
	{
		if (isalnum(*it) or (*it == '/') or (*it == '.') or (*it == '-') or (*it == '_'))
		{
			value.push_back(*it);
			continue ;
		}
		break ;
	}
	for (; it != node.getValue().end(); ++it)
	{
		if (not isspace(*it))
		{
			this->putError("location: invalid value", node.getLineNumber());
			return ;
		}
	}
	if (value == "")
	{
		this->putError("location: invalid value", node.getLineNumber());
		return ;
	}
	if (value[0] != '/')
	{
		this->putError("location: location needs to start with a `/`", node.getLineNumber());
		return ;
	}
	// Add reformat value: /../test/./coucou -> /test/coucou/
	if (value[value.size() - 1] != '/')
	{
		value.push_back('/');
	}
	if (server.getLocationPool().find(value) != server.getLocationPool().end())
	{
		this->putError("location: `" + value + "` is already defined", node.getLineNumber());
		return ;
	}
	this->setLocation(node, location);
	server.getLocationPool()[value] = location;
}

void	Configure::setLocation(ConfigTree const& node, Location& location)
{
	for (std::vector<ConfigTree>::const_iterator location_prop = node.getLeaves().begin();
		 location_prop != node.getLeaves().end();
		 ++location_prop
		)
	{
		if (location_prop->getKey() == "root")
		{
			this->addRoot(*location_prop, location);
			continue ;
		}
		this->putError(location_prop->getKey() + ": unknown property", node.getLineNumber());
	}
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

void	Configure::putError(std::string const& msg, size_t n_line)
{
	std::cerr << "Bad config: line " << n_line << ": " << msg << std::endl;
	_status = 1;
}

//addDuoIVS
void	Configure::addDuoIVS(std::string name, std::vector<VirtualServer*> list)
{
	this->duoIVS[name] = list;
}
