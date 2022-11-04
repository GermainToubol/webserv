// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   configure.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/04 11:47:09 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/04 15:57:51 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

/** @file */

#include <fstream>
#include <iostream>
#include <string>
#include "configure.hpp"

Configure::Configure(std::string const& file): filename(file), _status(0)
{
	std::ifstream ifs;

	ifs.open(filename.c_str());
	if (ifs.good())
	{
		_status = 0;
	}
	else
	{
		_status = 1;
	}
	ifs.close();
}

int Configure::status(void) const
{
	return (_status);
}
