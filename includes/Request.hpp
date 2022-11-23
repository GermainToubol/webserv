/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:17:54 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 20:53:02 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# pragma once

#include <string>
#include <map>

#include "Setup.hpp"

class Request
{
	public:
		Request(int fd);
		~Request();

		/*Accesseurs*/
		std::string const&	getBoundary() const;
		std::string const&	getContent() const;
		int			const&	getFd() const;
		void				setBoundary(std::string const& boundary);
		void				setContent(std::string const& content);
		void				setFd(int const& fd);

		/*Methodes*/
		

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
		std::string							_version;
		std::string							_body;
		std::map<std::string, std::string>	_fields;
};

#endif
