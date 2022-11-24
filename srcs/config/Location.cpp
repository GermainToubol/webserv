/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 15:50:42 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/24 15:52:22 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::string path): _path(path) {}

Location::~Location() {}

std::string	Location::getPath() const
{
	return (this->_path);
}