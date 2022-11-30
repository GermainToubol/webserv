/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:44:18 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/30 13:42:16 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# pragma once

# include <string>
# include <sstream>

# define BUFFER_SIZE 4096
# define REQUEST_TIMEOUT 5 //3min

template <typename T>
std::string to_string ( T Number )
{
	std::ostringstream	ss;

	ss << Number;
	return ss.str();
}

void	derror(std::string const& msg);

#endif