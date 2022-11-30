/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configure.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             */
/*   Updated: 2022/11/30 17:43:40 by lgiband          ###   ########.fr       */
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

typedef void (Configure::*t_server_func)(ConfigTree const&, VirtualServer&);
typedef void (Configure::*t_location_func)(ConfigTree const&, Location&);

typedef struct s_server_pair {
	std::string str;
	t_server_func fnc;
} t_server_pair;

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
	this->tree = NULL;
	duoIVS.clear();
}

std::vector<VirtualServer> const& Configure::getServers(void) const
{
	return (this->server_list);
}

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
		;
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
		this->duoIVS[current_server.getHost() + ":" + current_server.getPort()];
	}
	this->setDuoIVS();
}

void	Configure::setServerProperties(ConfigTree const& node, VirtualServer& server)
{
	const t_server_pair function_tab[] = {
		{"listen",		&Configure::addListen},
		{"root",		&Configure::addRoot},
		{"server_name",	&Configure::addServerName}
	};

	(void)function_tab;
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
		if (server_prop->getKey() == "index")
		{
			this->addIndex(*server_prop, server);
			continue ;
		}
		if (server_prop->getKey() == "permissions")
		{
			this->addPermission(*server_prop, server);
			continue ;
		}
		if (server_prop->getKey() == "max_body_size")
		{
			this->addMaxBodySize(*server_prop, server);
			continue ;
		}
		if (server_prop->getKey() == "autoindex")
		{
			this->addAutoindex(*server_prop, server);
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
	char buffer[INET_ADDRSTRLEN];

	(void)server;
	(void)hint;
	if (getaddrinfo(address.c_str(), server.getPort().c_str(), &hint, &res))
	{
		this->putError("listen: could not resolve `" + address + "`", line_nb);
		return (false);
	}
	inet_ntop(res->ai_family, &((struct sockaddr_in *)res->ai_addr)->sin_addr, buffer, INET_ADDRSTRLEN);
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
		{
			break ;
		}
	}
	root.assign(it, rit.base());
	if (root == "" or root[0] != '/')
	{
		return (this->putError("root: expect absolut path", node.getLineNumber()));
	}
	server.setRoot(root);
}

///////////////////////////////////////////////////////////////////////////////
//                                   Index                                   //
///////////////////////////////////////////////////////////////////////////////
template<class T>
void	Configure::addIndex(ConfigTree const& node, T& server)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string index;

	if (not node.getLeaves().empty())
	{
		this->putError("index: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("index: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	index.assign(it, rit.base());
	if (index == "")
	{
		return (this->putError("index: empty value", node.getLineNumber()));
	}
	server.setIndex(index);
}

///////////////////////////////////////////////////////////////////////////////
//                                 Autoindex                                 //
///////////////////////////////////////////////////////////////////////////////
template<class T>
void	Configure::addAutoindex(ConfigTree const& node, T& server)
{
	std::string::const_iterator it;
	std::string::const_reverse_iterator rit;
	std::string value;

	if (not node.getLeaves().empty())
	{
		this->putError("autoindex: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("autoindex: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	value.assign(it, rit.base());
	if (value == "on")
		return (server.setAutoindex(true));
	if (value == "off")
		return (server.setAutoindex(false));
	return (this->putError("autoindex: invalid value", node.getLineNumber()));
}

///////////////////////////////////////////////////////////////////////////////
//                                Server_name                                //
///////////////////////////////////////////////////////////////////////////////
// For now: only one name per server-block

void	Configure::addServerName(ConfigTree const& node, VirtualServer& server)
{
	std::string::const_iterator it;
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
//                                Permissions                                //
///////////////////////////////////////////////////////////////////////////////
template<class T>
void	Configure::addPermission(ConfigTree const& node, T& server)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string permissions;
	long n;
	char *pos;

	if (not node.getLeaves().empty())
	{
		this->putError("permissions: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("permissions: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	permissions.assign(it, rit.base());
	n = strtol(permissions.c_str(), &pos, 10);
	if (n < 0 or n >= 8 or *pos != '\0')
	{
		this->putError("permissions: invalid value", node.getLineNumber());
		return;
	}
	server.setPermissions(n);
}

///////////////////////////////////////////////////////////////////////////////
//                               Max Body Size                               //
///////////////////////////////////////////////////////////////////////////////
template<class T>
void	Configure::addMaxBodySize(ConfigTree const& node, T& server)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string size_str;
	long n;
	char *pos;

	if (not node.getLeaves().empty())
	{
		this->putError("max_body_size: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("max_body_size: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	size_str.assign(it, rit.base());
	n = strtol(size_str.c_str(), &pos, 10);
	if (size_str == "" or n < 0 or *pos != '\0')
	{
		this->putError("max_body_size: invalid value", node.getLineNumber());
		return;
	}
	server.setMaxBodySize(static_cast<size_t>(n));
}

///////////////////////////////////////////////////////////////////////////////
//                                  Redirect                                 //
///////////////////////////////////////////////////////////////////////////////
void	Configure::addRedirect(ConfigTree const& node, Location& location)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string redirect;

	if (not node.getLeaves().empty())
	{
		this->putError("redirection: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("redirection: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	redirect.assign(it, rit.base());
	if (redirect == "")
	{
		this->putError("redirection: invalid value", node.getLineNumber());
		return;
	}
	location.setRedirect(redirect);
}

///////////////////////////////////////////////////////////////////////////////
//                                 DefautFile                                //
///////////////////////////////////////////////////////////////////////////////
void	Configure::addDefaultFile(ConfigTree const& node, Location &location)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string file;

	if (not node.getLeaves().empty())
	{
		this->putError("default_file: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("default_file: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	file.assign(it, rit.base());
	if (file == "" or file[0] != '/')
	{
		return (this->putError("default_file: expect absolut path", node.getLineNumber()));
	}
	location.setDefaultFile(file);
}

///////////////////////////////////////////////////////////////////////////////
//                                  Post_dir                                 //
///////////////////////////////////////////////////////////////////////////////
void	Configure::addPostDir(ConfigTree const& node, Location &location)
{
	std::string::const_iterator it;
	std::string ::const_reverse_iterator rit;
	std::string file;

	if (not node.getLeaves().empty())
	{
		this->putError("post_dir: unexpected properties", node.getLineNumber());
		return ;
	}
	if (not node.hasDelimiter())
	{
		this->putError("post_dir: missing delimiter", node.getLineNumber());
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
		{
			break ;
		}
	}
	file.assign(it, rit.base());
	if (file == "" or file[0] != '/')
	{
		return (this->putError("post_dir: expect absolut path", node.getLineNumber()));
	}
	location.setPostDir(file);
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
		if (location_prop->getKey() == "index")
		{
			this->addIndex(*location_prop, location);
			continue ;
		}
		if (location_prop->getKey() == "permissions")
		{
			this->addPermission(*location_prop, location);
			continue ;
		}
		if (location_prop->getKey() == "max_body_size")
		{
			this->addMaxBodySize(*location_prop, location);
			continue ;
		}
		if (location_prop->getKey() == "autoindex")
		{
			this->addAutoindex(*location_prop, location);
			continue ;
		}
		if (location_prop->getKey() == "redirect")
		{
			this->addRedirect(*location_prop, location);
			continue ;
		}
		if (location_prop->getKey() == "default_file")
		{
			this->addDefaultFile(*location_prop, location);
			continue ;
		}
		if (location_prop->getKey() == "post_dir")
		{
			this->addPostDir(*location_prop, location);
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

bool	str_endswith(std::string const& str, std::string const& suffix)
{
	if (str.length() < suffix.length())
		return (false);
	else
		return (std::equal(str.rbegin(), suffix.rbegin(), suffix.rend()));
}

///////////////////////////////////////////////////////////////////////////////
//                                 SetDuoIVS                                 //
///////////////////////////////////////////////////////////////////////////////

void	Configure::setDuoIVS(void)
{
	std::string suffix;
	std::string fullname;

	for (
		std::vector<VirtualServer>::iterator server_it = this->server_list.begin();
		server_it != this->server_list.end();
		++server_it
		)
	{
		suffix = ":" + server_it->getPort();
		fullname = server_it->getHost() + suffix;
		for (
			std::map<std::string, std::vector<VirtualServer*> >::iterator inter_it = this->duoIVS.begin();
			inter_it != this->duoIVS.end();
			++inter_it
			)
		{
			if (str_endswith(inter_it->first, suffix))
			{
				if (inter_it->first == fullname or server_it->getHost() == "0.0.0.0")
					inter_it->second.push_back(&(*server_it));
			}
		}
	}
	for (std::map<std::string, std::vector<VirtualServer*> >::const_iterator it = this->duoIVS.begin(); it != this->duoIVS.end(); it++)
	{
		std::cout << " [ duoIVS (config side) ] " << it->first << " " << std::endl;
	}
}
