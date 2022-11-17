// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ConfigEntry.hpp                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/11/17 10:25:51 by gtoubol           #+#    #+#             //
//   Updated: 2022/11/17 12:30:17 by gtoubol          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef CONFIGENTRY_H
#define CONFIGENTRY_H
#pragma once

#include <cstddef>
#include <exception>
#include <string>

class ConfigEntry
{
public:
	ConfigEntry(std::string const&);
	virtual ~ConfigEntry(void) {}

	size_t		getLevel(void) const;
	std::string const& getKey(void) const;
	std::string const& getValue(void) const;
	bool		hasDelimiter() const;
	bool		isValueEmpty(void) const;

protected:
	bool	isLevelChar(const char) const;
	bool	isKeyChar(const char) const;
	bool	isDelimiter(const char) const;

	size_t		level;
	std::string	key;
	std::string value;
	bool		hasdelimiter;
};

#endif /* CONFIGENTRY_H */
