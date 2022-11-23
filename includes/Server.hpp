/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:09:47 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 12:33:01 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#pragma once

#include <string>
#include <vector>
#include <map>

#include "VirtualServer.hpp"
#include "Configure.hpp"

# define MAX_LISTEN 100
# define MAX_CLIENT 100

class WebServer
{
	public:
		WebServer(Configure const&);
		~WebServer(void);

		/*Accesseurs*/
		std::vector<VirtualServer> const& getVirtualServers() const;
		std::multimap<std::string, std::string> const& getMimeTypes() const;
		
		/*Init*/
		int	init();

	private:
		int										_epoll_fd;
		std::vector<VirtualServer>				_virtual_servers;
		std::multimap<std::string, std::string>	_mimetypes;
		std::map<int, std::string>				_status_codes;
};

#endif
