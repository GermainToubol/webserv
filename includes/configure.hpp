// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.hpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:42:29 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/04 15:56:04 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/** @file */

#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP
#pragma once

#include <string>

/**
 * @class Configure
 * @brief Tool to load and parse the configuration file.
 *
 * Wrapper around the lexer/parser of the configuration files (yaml style)
 */
class Configure
{
public:
	Configure(std::string const&);
	virtual ~Configure() {}

	int	status(void) const;

private:
	std::string	filename;
	int			_status;
};

#endif
