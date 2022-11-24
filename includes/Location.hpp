/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 15:46:41 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/24 21:27:10 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# pragma once

# include <string>
# include <map>

class Location
{
	public:
		Location();
		~Location();

		/*Accesseurs*/
		std::string		getRoot() const;
		std::string		getIndex() const;


	private:
		bool								_autoindex;
		std::string							_default_file;
		std::string							_root;
		std::string							_index;
		std::string							_post_dir;
		std::string							_permissions;
		std::string							_max_body_size;
		std::string							_redirect;
		std::string							_cgi_permissions;
		std::map<std::string, std::string>	_cgi_path;
		
};

#endif