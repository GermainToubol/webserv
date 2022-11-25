/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:17:54 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 16:37:15 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# pragma once

#include <string>
#include <vector>
#include <map>

#include "Setup.hpp"
#include "Location.hpp"

class Request
{
	public:
		Request(int fd);
		~Request();

		/*Accesseurs*/
		std::string const&	getBoundary() const;
		std::string const&	getContent() const;
		int			const&	getFd() const;
		Location	const	*getLocation() const;
		std::string const&	getMethod() const;
		std::string const&	getExtension() const;
		
		void				setBoundary(std::string const& boundary);
		void				setContent(std::string const& content);
		void				setFd(int const& fd);
	
		/*basicCheck*/
		int					basicCheck(Setup *setup);

		/*setUri*/
		int					setUri(Setup *setup);

		/*getLocation*/
		int					setLocation(Setup *setup);

		/*getServer*/
		int					setServer(Setup *setup, std::vector<VirtualServer*> const& server_pool);
		
		/*Parsing*/
		int					setFirstline(Setup *setup, std::string const& line);
		int					parsing(Setup *setup);

		/*Fonctions*/
		int					addContent(std::string const& content);

	private:
		int									_fd;

		std::string							_boundary;
		std::string							_content;

		std::string							_method;
		std::string							_uri;
		std::string							_extension;
		std::string							_query;
		std::string							_version;
		std::string							_body;
		std::map<std::string, std::string>	_fields;

		Location const 						*_location;
		std::string 						_location_path;
		
};

#endif
