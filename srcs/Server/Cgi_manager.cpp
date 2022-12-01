/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_manager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:55:51 by fmauguin          #+#    #+#             */
/*   Updated: 2022/12/01 08:16:29 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi_manager.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <algorithm>

#ifndef __GNUC__
#pragma region Constructor &&Destructor
#endif

Cgi_manager::Cgi_manager(Request *request, Configure *config) : _request(request), _config(config)
{
	this->_init();
	return;
}

Cgi_manager::Cgi_manager(Cgi_manager const &src)
{
	*this = src;
	return;
}

Cgi_manager::~Cgi_manager(void)
{
}

#ifndef __GNUC__
#pragma endregion Constructor &&Destructor
#endif

Cgi_manager &Cgi_manager::operator=(Cgi_manager const &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs.getRequest();
		this->_config = rhs.getConfig();
	}
	return *this;
}

Request *Cgi_manager::getRequest(void) const
{
	return this->_request;
}

Configure *Cgi_manager::getConfig(void) const
{
	return this->_config;
}
std::ostream &operator<<(std::ostream &o, Cgi_manager const &rhs)
{
	std::cout << "Cgi manager" << std::endl;
}

void Cgi_manager::_init(void)
{
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "WEBSERV/1.0";
	this->_env["REQUEST_METHOD"] = _request->getMethod();
	this->_env["CONTENT_TYPE"] = _request->_body;						// MIME TYPE, null if not known
	this->_env["CONTENT_LENGTH"] = to_string(_request->_body.length()); // content-body length
	this->_env["PATH_INFO"] = _config->getPath();						// cgi file
	this->_env["PATH_TRANSLATED"] = _config->getPath();					// cgi file

	this->_env["SCRIPT_NAME"] = _config->getCGIbin();		   // cgi binary
	this->_env["SCRIPT_FILENAME"] = this->_env["SCRIPT_NAME"]; // full pathname
	this->_env["REQUEST_URI"] = _request->getURI();
	this->_env["QUERY_STRING"] = _request->getQuery();

	this->_env["REMOTE_ADDR"] = _config->getAddr(); // Addr remote get from socket

	this->_env["REMOTE_IDENT"] = _request->getClient()->getLogin(); // Need class CLIENT link request and socket
	this->_env["REMOTE_USER"] = this->_env["REMOTE_IDENT"];

	this->_env["SERVER_NAME"] = _config->getServerName(); // get the good NODE from ConfigureTree
	this->_env["SERVER_PORT"] = _config->getServerPort(); // get the good NODE from ConfigureTree
}

char *convert(const std::string &s)
{
	char *pc = new char[s.size() + 1];
	std::strcpy(pc, s.c_str());
	return pc;
}

int Cgi_manager::execute()
{
	char **env;
	char *argv[3];
	int fd_pipe[2];
	int saveSTDIN;
	int saveSTDOUT;

	const char **env = new const char *[_env.size()];
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		std::string tmp = it->first + "=" + it->second;
		env[i] = convert(tmp);
		i++;
	}
	argv[0] = convert(_env["SCRIPT_NAME"]);
	argv[1] = convert(_env["SCRIPT_FILENAME"]);
	argv[2] = NULL;

	if (pipe(fd_pipe) != 0)
		return 1;

	saveSTDIN = dup(STDIN_FILENO);
	saveSTDOUT = dup(STDOUT_FILENO);
	pid_t pid = fork();

	if (pid == 0)
	{
		execve(argv[0], argv, env);
		exit(1);
	}
	else if (pid > 0)
	{
		close(fd_pipe[0]);
		close(fd_pipe[1]);

		int status;

		if (waitpid(fd_pipe, &status, 0) == -1) // Function interdite???
			return 1;
		if (WIFEXITED(status) && WEXITSTATUS(status))
			return 2;
	}
	else
		return 2;
	dup2(saveSTDIN, STDIN_FILENO);
	dup2(saveSTDOUT, STDOUT_FILENO);
	close(saveSTDIN);
	close(saveSTDOUT);
	// DONT FORGET THE DELETE
	return 0;
}
