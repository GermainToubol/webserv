/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:44:18 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/29 14:37:46 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# pragma once

# include <string>
# include <sstream>

# define BUFFER_SIZE 4096

template <typename T>
std::string to_string ( T Number )
{
	std::ostringstream	ss;

	ss << Number;
	return ss.str();
}

void	derror(std::string const& msg);

#endif