/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:17:54 by lgiband           #+#    #+#             */
/*   Updated: 2022/12/04 19:29:23 by lgiband          ###   ########.fr       */
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
#include "utils.hpp"

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
		std::string const&	getUri() const;
		std::string const&	getField(std::string) const;
		std::string const&	getBody() const;
		bool		const&	getChunkedMode() const;
		
		void				setBoundary(std::string const& boundary);
		void				setContent(std::string const& content);
		void				setFd(int const& fd);
		void				setChunkedMode(bool const& mode);
		void				addBody(char *buffer, int size);
		int					addChunkedData(const char *buffer, int size);
	
		void				replaceAllBody(std::string const&, std::string const&);
	
		/*basicCheck*/
		bool				isValidUri(std::string const& uri);
		int					basicCheck(Setup *setup);

		/*setUri*/
		int					setUri(Setup *setup);

		/*getLocation*/
		int					setLocation(Setup *setup);

		/*getServer*/
		int					setServer(Setup *setup, std::vector<VirtualServer*> const* server_pool);
		
		/*Parsing*/
		int					setFirstline(Setup *setup, std::string const& line);
		int					parsing(Setup *setup);

		/*Fonctions*/
		int					addContent(char *buf, int ret);

	private:
		int									_fd;
		bool								_chunked_mode;
		std::string::size_type				_chunked_size;
		std::string							_chunked_buffer;

		std::string							_boundary;
		std::string							_content;
		size_t								_content_size;
		size_t								_is_header;

		std::string							_method;
		std::string							_uri;
		std::string							_extension;
		std::string							_query;
		std::string							_version;
		std::string							_body;
		std::map<std::string, std::string>	_fields;

		Location const 						*_location;
		std::string 						_location_path;

		std::string							_empty;		
};
#endif
