/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:09:47 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/24 15:06:33 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#pragma once

#include <string>
#include <vector>
#include <map>

#include "VirtualServer.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Configure.hpp"

# define MAX_LISTEN 100
# define MAX_CLIENTS 100
# define TIMEOUT 200
# define BUFFER_SIZE 4096

class WebServer
{
	public:
		WebServer(Configure const&);
		~WebServer(void);

		/*Accesseurs*/
		std::vector<VirtualServer>				const& getVirtualServers() const;
		std::multimap<std::string, std::string>	const& getMimeTypes() const;
		std::vector<VirtualServer>				const& getAccessibleServer(int client_fd) const;
		
		/*Init*/
		int	addDuoCS(int client, int server);
		int	create_socket(std::string port, std::string ip);
		int	init(void);

		/*Run*/
		int	send_response(int fd);
		int buildResponse(int fd, Request *request, Setup const &setup);
		int	setResponse(int fd, Request *request);
		int	get_request(int fd);
		int	new_connection(int fd);
		int	event_loop(struct epoll_event *events, int nb_events);
		int	run(void);

		/*Utils*/
		void	remove_fd_request(int fd);
		Request	*get_fd_request(int fd);
		int		is_server(int fd);
		
	private:
		int													_epoll_fd;
		std::vector<VirtualServer>							_virtual_servers;
		std::vector<Request>								_all_request;
		std::vector<Response>								_all_response;
		std::multimap<std::string, std::string>				_mimetypes;
		std::map<int, std::string>							_status_codes;
		std::map<std::string, std::vector<VirtualServer> >	_duoIVS;
		std::map<int, std::string>							_duoSI;
		std::map<int, int>									_duoCS;
		char												_buffer[BUFFER_SIZE + 1];
};

#endif
