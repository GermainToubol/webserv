/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 11:56:16 by gtoubol           #+#    #+#             */
/*   Updated: 2022/11/25 14:57:15 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP
#pragma once

#include <map>
#include <string>
#include <map>
#include <vector>

#include "Location.hpp"

class VirtualServer
{
public:
	VirtualServer(void);
	virtual ~VirtualServer(void) {}

	std::string 						const& getServerName() const;
	std::string 						const& getRoot() const;
	std::string 						const& getHost() const;
	std::string 						const& getPort() const;
	int 								const& getFd()	const;
	std::map<std::string, Location>		const& getLocationPool() const;
	std::map<std::string, Location>&	getLocationPool();
	std::map<int, std::string>			const& getErrorPage() const;

	void	setServerName(std::string const&);
	void	setRoot(std::string const&);
	void	setHost(std::string const&);
	void	setPort(std::string const&);
	void	setFd(int const&);

	std::string const& getIndex(void) const;
	void	setIndex(std::string const&);

	int		getPermissions(void) const;
	void	setPermissions(int);

private:
	int			fd;
	std::string	server_name;
	std::string	root;
	std::string	host;
	std::string	port;
	std::string index;
	int			permissions;


	std::map<std::string, Location> location_pool;
	std::map<int, std::string> error_page;
	//permissions -default G
	//max_body_size
	//autoindex
	//locations
		//redirect
		//root
		//index
		//autoindex
		//default_file directory
		//permissions
		//post_dir
		//cgi permissions (.php/.py....)
		//max_body_size
};

#endif /* VIRTUALSERVER_H */
