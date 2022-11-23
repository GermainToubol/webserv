/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:59:37 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 17:57:28 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>

#include <signal.h>

#include "WebServer.hpp"
#include "Configure.hpp"

int	running = 1;

void	get_sig(int sig)
{
	if (sig == SIGINT)
	{
		std::cerr << "STOP" << std::endl;
		running = 0;
		return ;
	}
}

struct sigaction	init_sig(void handler(int), int sig)
{
	struct sigaction	act;

	std::memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	sigaction(sig, &act, NULL);
	return (act);
}

int	main(void)
{
	Configure config("test_config");

	init_sig(get_sig, SIGINT);
	
	if (!config.isGood())
		return (1);
	
	WebServer server(config);
	if (server.init())
		return (1);
	
	Request request(4);

	request.addContent("GET / HTTP/1.1\r\nHost: localhost:8080\r\nAccept: */*\r\n\r\nAlelouia");
	request.parsing();
	
	return (0);
}