/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_manager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:55:51 by fmauguin          #+#    #+#             */
/*   Updated: 2022/12/04 14:04:19 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>

#include <unistd.h>

#include "Cgi_manager.hpp"
#include "utils.hpp"

extern int flags;

#ifndef __GNUC__
#pragma region Constructor &&Destructor
#endif

Cgi_manager::Cgi_manager(Request *request, Setup *setup, std::string const& client_ip, std::string const& cgi_exe) :
	_request(request),
	_setup(setup),
	_client_ip(client_ip),
    _cgi_exe(cgi_exe)
{
	this->_content_type = request->getField("Content-Type");
    if (this->_content_type == "")
        {this->_content_type = "text/plain";}
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
		this->_setup = rhs.getSetup();
	}
	return *this;
}

Request *Cgi_manager::getRequest(void) const
{
	return this->_request;
}

Setup *Cgi_manager::getSetup(void) const
{
	return this->_setup;
}
std::ostream &operator<<(std::ostream &o, Cgi_manager const &rhs)
{
	(void)rhs;
	o << "Cgi manager" << std::endl;
	return o;
}

void Cgi_manager::_init(void)
{
    this->_env["REDIRECT_STATUS"] = "200";
    this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->_env["SERVER_SOFTWARE"] = "WEBSERV/1.0";
    this->_env["REQUEST_METHOD"] = _request->getMethod();
    this->_env["CONTENT_TYPE"] = _content_type;                        // MIME TYPE, null if not known
    this->_env["CONTENT_LENGTH"] = _request->getField("Content-Length"); // If content is empty ==> 
    if (this->_env["CONTENT_LENGTH"] == "")
        this->_env["CONTENT_LENGTH"] = "0";
    this->_env["PATH_INFO"] = _setup->getUri();                        // cgi file
    this->_env["PATH_TRANSLATED"] = _setup->getUri();                    // cgi file
    this->_env["REQUEST_URI"] = _setup->getUri();                        // cgi file
    this->_env["QUERY_STRING"] = _setup->getQuery();
    this->_env["SCRIPT_NAME"] = this->_cgi_exe;       // cgi binary
    
    if (flags & FLAG_VERBOSE)
        std::cerr << "[ CGI uri: " << _setup->getUri() << " ]" << std::endl;

    this->_env["SCRIPT_FILENAME"] = _setup->getUri(); // full pathname

     this->_env["REMOTE_ADDR"] = this->_client_ip; // Addr remote get from socket
     this->_env["REMOTE_HOST"] = this->_client_ip; // Addr remote get from socket

    //if AUTH
    // this->_env["REMOTE_IDENT"] = _request->getClient()->getLogin(); // Need class CLIENT link request and socket
    // this->_env["REMOTE_USER"] = this->_env["REMOTE_IDENT"];

    this->_env["SERVER_NAME"] = _setup->getServer()->getServerName(); // get the good NODE from ConfigureTree
    this->_env["SERVER_PORT"] = _setup->getServer()->getPort(); // get the good NODE from ConfigureTree
}

char *convert(const std::string &s)
{
	char *pc = new char[s.size() + 1];
	std::strcpy(pc, s.c_str());
	return pc;
}

int Cgi_manager::execute(int *cgi_fd)
{
    char *argv[3];
    int fd_pipe[2];
    int saveSTDIN;
    int saveSTDOUT;
    std::string tmp;

    std::cout << "======================CGI EXECUTE===========================" << std::endl;
    char **env = new char*[_env.size() + 1];
    int i = 0;
    std::string *env_arr = new std::string[_env.size() + 1];
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    {
        tmp = it->first + "=" + it->second;
        env_arr[i] = it->first + "=" + it->second;
        env[i] = &env_arr[i][0];
        std::cerr<<env[i] << std::endl;
        i++;
    }
    env[i] = NULL;
    argv[0] = &_env["SCRIPT_NAME"][0];
    argv[1] = &_env["SCRIPT_FILENAME"][0];
    argv[2] = NULL;

    if (pipe(fd_pipe) != 0)
        return 500;
    *cgi_fd = fd_pipe[0]; //j'imagine que c ca qu'il faut faire
    saveSTDIN = dup(STDIN_FILENO);
    saveSTDOUT = dup(STDOUT_FILENO);
    pid_t pid = fork();

    if (pid == 0)
    {
        if (dup2(fd_pipe[1], 1) == -1)
            return (500);
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        std::cerr << "[ EXECVE ]" << std::endl;
        execve(argv[0], argv, env);
        std::cerr << "[ EXECVE FAIL ]" << std::endl;

        delete[] env;
        delete[] env_arr;
        exit(1);
    }
    else if (pid < 0)
        return 500;
    close(fd_pipe[1]);
    delete[] env;
    delete[] env_arr;
    dup2(saveSTDIN, STDIN_FILENO);
    dup2(saveSTDOUT, STDOUT_FILENO);
    close(saveSTDIN);
    close(saveSTDOUT);
    return 0;
}