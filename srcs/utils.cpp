/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 16:34:10 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/30 20:45:02 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <stack>
#include <string>

std::string	reformatUri(std::string const& uri)
{
	std::string				new_uri;
	std::string::size_type	start = 0;
	std::string::size_type	end = 0;
	std::stack<std::string>	stack;
	std::string				tmp;

	if (uri.find("/") == std::string::npos)
	{
		new_uri = uri;
		return (new_uri);
	}
	while (end != uri.size())
	{
		if (start != 0)
			start = uri.find('/', end);
		while (uri.find('/', start + 1) == 0)
			start++;
		if (uri[start] == '/')
			start++;
		end = uri.find('/', start);
		if (end == std::string::npos)
			end = uri.size();
		tmp = uri.substr(start, end - start);
		if (tmp == "..")
		{
			if (!stack.empty())
				stack.pop();
		}
		else if (tmp != "." && tmp != "")
			stack.push(tmp);
		start = end;
	}
	while (!stack.empty())
	{
		new_uri = stack.top() + "/" + new_uri;
		stack.pop();
	}
	if (uri[0] == '/')
		new_uri = "/" + new_uri;
	if (uri[uri.size() - 1] == '/' && new_uri[new_uri.size() - 1] != '/')
		new_uri = new_uri + "/";
	else if (uri[uri.size() - 1] != '/' && new_uri[new_uri.size() - 1] == '/')
		new_uri = new_uri.erase(new_uri.size() - 1);
	if (new_uri.size() == 0)
		new_uri = "/";
	return (new_uri);
}
