// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   VirtualServer.hpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/17 11:56:16 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/17 14:37:33 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef VIRTUALSERVER_H
#define VIRTUALSERVER_H
#pragma once

#include <string>

struct VirtualServer
{
	std::string server_name;
	std::string root;
	unsigned long int	host;
	unsigned int		port;
};


#endif /* VIRTUALSERVER_H */
