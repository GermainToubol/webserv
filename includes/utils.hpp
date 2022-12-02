/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:44:18 by lgiband           #+#    #+#             */
/*   Updated: 2022/12/02 13:46:51 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# pragma once

# include <string>
# include <sstream>

# define BUFFER_SIZE 4096
# define REQUEST_TIMEOUT 5 //3min
# define WEBSERV_COOKIE "WebServCookie="
# define BASE_GENERATOR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

template <typename T>
std::string to_string ( T Number )
{
	std::ostringstream	ss;

	ss << Number;
	return ss.str();
}

void		derror(std::string const& msg);
std::string uriDecode(const std::string &src);
std::string	reformatUri(std::string const& uri);
std::string	generateRandomCookie(int size, std::string base);

bool	doesPathExist(std::string const& path);
bool	isPathReadable(std::string const& path);
bool	isPathWriteable(std::string const& path);

bool	isDirectory(std::string const& path);
bool	isFile(std::string const& path);
#endif