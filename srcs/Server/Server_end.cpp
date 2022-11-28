/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_end.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 20:20:45 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/28 20:24:41 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include <unistd.h>

#include "WebServer.hpp"
#include "Cache.hpp"

int	WebServer::end(void)
{
	std::cout << "Closing server..." << std::endl;
	
	close(this->_epoll_fd);
	//close cache
	// close client_fd
	// for (std::vector<Cache>::iterator it = this->_all_cache.begin(); it != this->_all_cache.end(); it++)
	// {
	// 	close(it->getFd());
	// }
}