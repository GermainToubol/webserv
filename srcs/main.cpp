/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:59:37 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/28 15:24:25 by lgiband          ###   ########.fr       */
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

void	derror(std::string const& msg)
{
	std::cerr << msg << std::endl;
}

int	main(void)
{
	Configure config("test_config");

	VirtualServer	vs = config.getServers()[0];
	Location		loc;



	vs.addLocation("/", loc);
	std::vector<VirtualServer *> vec;

	vec.push_back(&vs);

	config.addDuoIVS(vs.getHost() + ":" + vs.getPort(), vec);
	std::cerr << vs.getRoot() << std::endl;

	init_sig(get_sig, SIGINT);
	
	if (!config.isGood())
		return (1);
	
	WebServer server(config);
	if (server.init())
		return (1);
	server.run();
	
	return (0);
}