// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ConfigTree.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/23 11:34:12 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/25 18:12:41 by gtoubol          ###   ########.fr       //
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

std::vector<ConfigTree> const& ConfigTree::getLeaves() const
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


std::string const& ConfigTree::getKey() const
{
	return (this->key);
}
std::string const& ConfigTree::getValue() const
{
	return (this->value);
}

bool		ConfigTree::hasDelimiter() const
{
	return (this->delimiter);
}
