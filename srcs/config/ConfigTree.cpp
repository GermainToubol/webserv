// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ConfigTree.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/23 11:34:12 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/23 11:34:12 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

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
