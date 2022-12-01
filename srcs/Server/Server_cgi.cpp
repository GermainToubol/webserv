/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 15:18:26 by fmauguin          #+#    #+#             */
/*   Updated: 2022/12/01 15:21:21 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

bool	WebServer::isCgi(int file_fd)
{
	if (this->_cgiFD.find(file_fd) != this->_cgiFD.end())
		return (true);
	return (false);
}

int	WebServer::cgiResponse(int file_fd)
{

}
