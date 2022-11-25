/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:39:45 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 13:48:06 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# pragma once

#include <string>
#include <vector>

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
		void					setBody(std::string const& body);
		void					setBodySize(std::string::size_type const& body_size);
		void					setPosition(std::string::size_type const& position);
		void					setFd(int const& fd);
		void					setStatus(int const& status);

	private:
		std::string				_filename;
		std::string				_header;
		std::string				_body;
		std::string::size_type	_body_size;
		std::string::size_type	_position;
		int						_fd;
		int						_status;
};

#endif