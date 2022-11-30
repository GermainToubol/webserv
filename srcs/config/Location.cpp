/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 15:50:42 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/30 12:24:09 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(): _permissions(GET_PERM | POST_PERM | DEL_PERM), _autoindex(1), _default_file(""),
					_root("/"), _index("index.html"), _post_dir("/"),
					 _redirect(""), _max_body_size(32000000)  {}

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

bool	const& Location::getAutoindex() const
{
	return (this->_autoindex);
}

std::string	const& Location::getDefaultFile() const
{
	return (this->_default_file);
}

std::string	const& Location::getPostDir() const
{
	return (this->_post_dir);
}

std::string::size_type	const& Location::getMaxBodySize() const
{
	return (this->_max_body_size);
}
