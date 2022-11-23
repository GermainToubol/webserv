/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:53:59 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 13:55:56 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

int	WebServer::is_server(int fd)
{
	for (std::vector<VirtualServer>::iterator it = this->_virtual_servers.begin(); it != this->_virtual_servers.end(); it++)
	{
		if (it->getFd() == fd)
			return (fd);
	}
	return (-1);
}