/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:59:37 by lgiband           #+#    #+#             */
/*   Updated: 2022/12/02 17:09:53 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>

#include <signal.h>

#include "WebServer.hpp"
#include "Configure.hpp"
#include "utils.hpp"

int		running = 1;
int		flags = 0;

void	get_sig(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "STOP" << std::endl;
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
	if (flags & FLAG_VERBOSE)
		std::cerr << msg << std::endl;
}

void	display_usage(void)
{
	std::cerr << "Usage: ./webserv [config_file] <-vt>" << std::endl;
	std::cerr << "	-v: verbose mode" << std::endl;
	std::cerr << "	-t: test mode" << std::endl;
}

int	get_flags(char **argv, int argc)
{
	t_flags		all_flags[][2] = {{'v', FLAG_VERBOSE},{'t', FLAG_TEST}};

	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] != '-' || argv[i][1] == '\0')
			return (1);
		for (int j = 1; argv[i][j]; j++)
		{
			for (int k = 0; k < (int)(sizeof(all_flags) / sizeof(all_flags[0])); k++)
			{
				if (argv[i][j] == all_flags[k]->character)
				{
					flags |= all_flags[k]->value;
					break ;
				}
				if (k == sizeof(all_flags) / sizeof(all_flags[0]) - 1)
					return (1);
			}
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	std::string filename("test_config");
	int			ret = 0;

	if (argc >= 2)
		filename = argv[1];
	if (argc >= 3)
		ret = get_flags(&argv[2], argc - 2);
	if (ret)
		return (display_usage(), 1);
	std::cout << argc <<" "<< (int)flags << " " << ret << std::endl;
	Configure config(filename);

	if (!config.isGood())
		return (1);

	if (flags & FLAG_TEST)
		return (0);
	init_sig(get_sig, SIGINT);
	
	WebServer server(config);
	if (server.init())
		return (1);
	server.run();
	server.end();

	return (0);
}
