/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:39:45 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/28 11:01:46 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# pragma once

#include <string>
#include <vector>
#include <map>

#include "Setup.hpp"

class Response
{
	public:
		Response();
		~Response();

		/*Accesseurs*/
		std::string 			const&	getFilename() const;
		std::string 			const&	getHeader() const;
		std::string 			const&	getBody() const;
		std::string::size_type	const&	getBodySize() const;
		std::string::size_type	const&	getPosition() const;
		int						const&	getFd() const;
		int						const&	getStatus() const;
		
		void					setFilename(std::string const& filename);
		void					setHeader(std::string const& header);
		void					setHeader(Setup *setup, std::map<int, std::string> const& status_code, std::multimap<std::string, std::string> const& mime_type, size_t body_size);
		void					setBody(std::string const& body);
		void					setBody(int code, std::string const& type);
		void					setBodySize(std::string::size_type const& body_size);
		void					setPosition(std::string::size_type const& position);
		void					setFd(int const& fd);
		void					setStatus(int const& status);

		/*Fonctions*/
		void	eraseHeader(int start, int end);
		void	eraseBody(int start, int end);
		int		setListingBody(std::string uri, std::string const& root);

	private:
		int						_fd;
		int						_send_status;
		std::string				_header;
		std::string::size_type	_body_size;
		std::string::size_type	_position;
		
		std::string				_body;
		
		std::string				_filename;

};

#endif
