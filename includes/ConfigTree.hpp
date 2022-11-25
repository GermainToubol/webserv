// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ConfigTree.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/23 11:17:36 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/23 21:22:13 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef CONFIGTREE_H
#define CONFIGTREE_H
#pragma once

#include <string>
#include <vector>
#include "ConfigEntry.hpp"

struct ConfigTree
{
public:
	ConfigTree();
	ConfigTree(ConfigEntry const&);
	virtual ~ConfigTree() {}

	void	print(std::string const&) const;
	std::vector<ConfigTree>& getLeaves();

private:
	std::string	key;
	std::string	value;
	bool		delimiter;
	std::vector<ConfigTree>	leaves;
};

#endif /* CONFIGTREE_H */
