// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   VirtualServer.hpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/17 11:56:16 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/17 16:02:20 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef VIRTUALSERVER_H
#define VIRTUALSERVER_H
#pragma once

#include <string>

class VirtualServer
{
public:
	VirtualServer(void);
	virtual ~VirtualServer(void) {}

	std::string const& getServerName() const;
	std::string const& getRoot() const;
	std::string const& getHost() const;
	std::string const& getPort() const;

	void	setServerName(std::string const&);
	void	setRoot(std::string const&);
	void	setHost(std::string const&);
	void	setPort(std::string const&);

private:
	std::string server_name;
	std::string root;
	std::string	host;
	std::string	port;
};


#endif /* VIRTUALSERVER_H */
