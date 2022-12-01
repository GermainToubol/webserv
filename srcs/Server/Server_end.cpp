/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_end.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 20:20:45 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/29 09:56:14 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

#include <unistd.h>

#include "WebServer.hpp"
#include "Cache.hpp"

int	WebServer::end(void)
{
	std::cout << "Closing server..." << std::endl;
	
	close(this->_epoll_fd);
	
	std::cerr << this->_all_cache.size() << std::endl;
	std::cerr << this->_all_request.size() << std::endl;
	std::cerr << this->_all_response.size() << std::endl;

	for (std::vector<Cache>::iterator it = this->_all_cache.begin(); it != this->_all_cache.end(); it++)
	{
		it->getStream()->close();
		delete it->getStream();
	}
	for (std::vector<Request>::iterator it = this->_all_request.begin(); it != this->_all_request.end(); it++)
		close(it->getFd());
	for (std::vector<Response>::iterator it = this->_all_response.begin(); it != this->_all_response.end(); it++)
		close(it->getFd());
	return (0);
}