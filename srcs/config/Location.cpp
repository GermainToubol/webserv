/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 15:50:42 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/25 12:04:12 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {}

Location::~Location() {}

std::string	const& Location::getRoot() const
{
	return (this->_root);
}

std::string	const& Location::getIndex() const
{
	return (this->_index);
}

std::string	const& Location::getRedirect() const
{
	return (this->_redirect);
}

int	const& Location::getPermission() const
{
	return (this->_permissions);
}

std::map<std::string, std::string>	const& Location::getCgiPerm() const
{
	return (this->_cgi_path);
}