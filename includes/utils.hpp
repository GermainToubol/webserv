/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:44:18 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 11:44:42 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# pragma once

# include <string>
# include <sstream>

template <typename T>
std::string to_string ( T Number )
{
	std::ostringstream	ss;

	ss << Number;
	return ss.str();
}

#endif