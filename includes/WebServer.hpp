/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:09:47 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 20:51:02 by lgiband          ###   ########.fr       */
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
		VirtualServer							const& getEntryServer(int fd) const;
		
		/*Init*/
		int	addDuoCS(int client, int server);
		int	create_socket(std::string port, std::string ip);
		int	init(void);

		/*Run*/
		int	send_response(int fd);
		int build_response(int fd, Request *request, Setup const &setup);
		int	set_response(int fd, Request *request, VirtualServer const &entry_server);
		int	get_request(int fd);
		int	new_connection(int fd);
		int	event_loop(struct epoll_event *events, int nb_events);
		int	run(void);

		/*Utils*/
		void	remove_fd_request(int fd);
		Request	*get_fd_request(int fd);
		int		is_server(int fd);
		
	private:
		int										_epoll_fd;
		std::vector<VirtualServer>				_virtual_servers;
		std::vector<Request>					_all_request;
		std::vector<Response>					_all_response;
		std::multimap<std::string, std::string>	_mimetypes;
		std::map<int, std::string>				_status_codes;
		std::map<int, int>						_duoCS;
		char									_buffer[BUFFER_SIZE + 1];
};

#endif
