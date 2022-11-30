/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:44:18 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/30 17:02:09 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# pragma once

# include <string>
# include <sstream>

# define BUFFER_SIZE 4096
# define REQUEST_TIMEOUT 180 //3min

template <typename T>
std::string to_string ( T Number )
{
	std::ostringstream	ss;

	ss << Number;
	return ss.str();
}

void		derror(std::string const& msg);
std::string	reformatUri(std::string const& uri);

#endif