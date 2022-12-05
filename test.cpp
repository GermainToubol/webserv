/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 16:34:10 by lgiband           #+#    #+#             */
/*   Updated: 2022/12/05 10:52:39 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <stack>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>

std::string				 _content = "";
std::string::size_type	 _content_size = 0;
std::string 			_chunked_buffer = "";
std::string::size_type	_chunked_size = 0;

int	addChunkedData(const char *buffer, int size)
{
	std::string::size_type	pos;
	std::string				size_data;
	std::string				line;
	std::string				tmp;

	line = buffer;

	if (size == 0)
		return (1);

	if (_chunked_size > 0)
	{
		if (line.size() > _chunked_size)
		{
			_content.append(line, 0, _chunked_size);
			std::cout << "content: " << _content << std::endl;
			tmp = line.substr(_chunked_size);
			_chunked_size = 0;
			_content_size = _content.size();
			return (addChunkedData(tmp.c_str(), tmp.size()));
		}
		else
		{
			_content.append(line);
			_chunked_size -= line.size();
			_content_size = _content.size();
		}
	}

	else if (_chunked_size == 0)
	{
		_chunked_buffer += line;
		pos = _chunked_buffer.find("\r\n");
		if (pos == 0)
		{
			_chunked_buffer.erase(0, 2);
			pos = _chunked_buffer.find("\r\n");
		}
		if (pos == std::string::npos)
			return (0);
		size_data = _chunked_buffer.substr(0, pos);
		_chunked_buffer.erase(0, pos + 2);
		_chunked_size = std::strtol(size_data.c_str(), NULL, 16);
		if (_chunked_size == 0)
			return (1);
		tmp = _chunked_buffer;
		_chunked_buffer = "";
		return (addChunkedData(tmp.c_str(), tmp.size()));
	}
	return(0);
}

int main()
{
	std::string test = "3e8\r\neeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
	std::string test2 = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\r\n0\r\n\r\n";

	//std::string test_end = "0\r\n\r\n";

	std::cout << "chunked size : " << _chunked_size << std::endl;
	addChunkedData(test.c_str(), test.size());
	std::cout << "chunked size : " << _chunked_size << std::endl;
	addChunkedData(test2.c_str(), test2.size());
	std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test3.c_str(), test3.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test4.c_str(), test4.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test5.c_str(), test5.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test6.c_str(), test6.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test7.c_str(), test7.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test8.c_str(), test8.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test9.c_str(), test9.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test10.c_str(), test10.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test11.c_str(), test11.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;
	//addChunkedData(test_end.c_str(), test_end.size());
	//std::cout << "chunked size : " << _chunked_size << std::endl;


	std::cout << _content << std::endl;
	
}