/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:09:47 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/30 13:44:28 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>

#include "VirtualServer.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Configure.hpp"
#include "Cache.hpp"

# define MAX_LISTEN 100
# define MAX_CLIENTS 100
# define TIMEOUT 200
# define SEND_SIZE 1048575

class WebServer
{
	public:
		WebServer(Configure const&);
		~WebServer(void);

		/*Accesseurs*/
		std::vector<VirtualServer>				const& getVirtualServers() const;
		std::multimap<std::string, std::string>	const& getMimeTypes() const;
		std::vector<VirtualServer*>				const*	getAccessibleServer(int client_fd) const;
		Response 										*getResponse(int client_fd) const;
		Cache											*getCache(std::string const& filename) const;
		
		/*End*/
		int	end(void);
		
		/*Init*/
		int	addDuoCS(int client, int server);
		int	create_socket(std::string port, std::string ip);
		int	init(void);

		/*BuildResponse*/
		int	openFile(Setup *setup, Response *response);
		int buildResponseDefault(int fd, Request *request, Setup *setup);
		int buildResponseListing(Request *request, Setup *setup, int client_fd);
		int buildResponseGet(Request *request, Setup *setup, int client_fd);

		/*Mode*/
		int	cgiMode(Request *request, Setup *setup, int client_fd);
		int	redirectMode(Request *request, Setup *setup, int client_fd);
		int	getMode(Request *request, Setup *setup, int client_fd);
		int	postMode(Request *request, Setup *setup, int client_fd);
		int	deleteMode(Request *request, Setup *setup, int client_fd);
		int	modeChoice(Request *request, Setup *setup, int client_fd);

		/*SendResponse*/
		int	removeResponse(int client_fd);
		int	sendHeader(int client_fd, Response *response);
		int	sendBody(int client_fd, Response *response);
		int	sendFile(int client_fd, Response *response);

		/*Post*/
	
		int	sendPostResponse(Request *request, Setup *setup, int client_fd);
		int	setPostUri(Request *request, Setup *setup);
		int	checkPostRequest(Request *request, Setup *setup);

		std::string	parseChamp(Setup *setup, Request *request, std::string const& str);
		int			urlEncodedPost(Request *request, Setup *setup);
		int			multipartPost(Request *request, Setup *setup);
		int			plainTextPost(Request *request, Setup *setup);
	
		/*Run*/
		int	sendResponse(int fd);
		int	setResponse(int fd, Request *request);
		int	getRequest(int fd);
		int	newConnection(int fd);
		int	event_loop(struct epoll_event *events, int nb_events);
		int	run(void);

		/*Utils*/
		bool	doesPathExist(std::string const& path);
		bool	isPathReadable(std::string const& path);
		bool	isPathWriteable(std::string const& path);

		bool	isDirectory(std::string const& path);
		bool	isFile(std::string const& path);
		void	remove_fd_request(int fd);
		Request	*get_fd_request(int fd);
		int		is_server(int fd);
		int		isMe(std::string const& uri, std::string const& path, std::string const& host);
		void	clearCache(void);
		void	clearTimeout(void);

		std::vector<std::string>	splitFormdata(std::string const& file, std::string const& boundary);

		
	private:
		int													_epoll_fd;
		std::vector<VirtualServer>							_virtual_servers;
		std::vector<Request>								_all_request;
		std::vector<Response>								_all_response;
		std::vector<Cache>									_all_cache;
		std::multimap<std::string, std::string>				_mimetypes;
		std::map<int, std::string>							_status_codes;
		std::map<std::string, std::vector<VirtualServer*> >	_duoIVS;
		std::map<int, std::string>							_duoSI;
		std::map<int, int>									_duoCS;
		std::map<int, std::pair<int, int> >	_timeout;
		char												_buffer[BUFFER_SIZE + 1];
		char												_send_buffer[SEND_SIZE + 1];
};

#endif
