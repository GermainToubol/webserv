/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Setup.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 20:06:50 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 20:17:06 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETUP_HPP
# define SETUP_HPP

# pragma once

# include <string>

# include "VirtualServer.hpp"

class Setup
{
	public:
		Setup();
		~Setup();

		/*Accesseurs*/
		int			const&	getCode() const;
		std::string const&	getUri() const;
		std::string const&	getFields() const;
		VirtualServer const&	getServer() const;

		void				setCode(int const& code);
		void				setUri(std::string const& uri);
		void				setFields(std::string const& fields);
		void				setServer(VirtualServer& server);

	private:
		int				_code;
		std::string		_uri;
		std::string		_fields;
		VirtualServer	*_server;
};

#endif