// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ConfigTree.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/23 11:34:12 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/23 21:38:02 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <string>
#include <vector>
#include "ConfigEntry.hpp"
#include "ConfigTree.hpp"

ConfigTree::ConfigTree()
{

}

ConfigTree::ConfigTree(ConfigEntry const& entry): key(entry.getKey()), value(entry.getValue()), delimiter(entry.hasDelimiter())
{

}

std::vector<ConfigTree>& ConfigTree::getLeaves()
{
	return (leaves);
}

void	ConfigTree::print(std::string const& spacer) const
{
	std::cout << spacer << this->key << std::endl;
	for (std::vector<ConfigTree>::const_iterator it = this->leaves.begin();
		 it != leaves.end(); ++it)
	{
		it->print(spacer + "  ");
	}
}
