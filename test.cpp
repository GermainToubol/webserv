/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 16:34:10 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/30 20:47:03 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <stack>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>

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



int main()
{
	struct epoll_event	events[100];
	int					nb_events;
	struct epoll_event	event;
	char buffer[1024];
	int re;
	char *env[] = {NULL};
	char cmd[] = "bash";
	char *args[] = {cmd, NULL};

	init_sig(get_sig, SIGINT);
	std::cerr << "\n=====================INIT====================\n" << std::endl;

	std::memset(&event, 0, sizeof(event));
	int _epoll_fd = epoll_create1(0);

	std::cerr << "\n=====================RUN=====================\n" << std::endl;
	
	int fdp[2];
	pipe(fdp);
	if (fork() == 0)
	{
		dup2(fdp[1], 1);
		close(fdp[0]);
		close(fdp[1]);
		execve("/usr/bin/bash", args, env);
	}
	close(fdp[1]);
	std::memset(&event, 0, sizeof(event));
	event.data.fd = fdp[0];
 	event.events = EPOLLIN;
 	epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fdp[0], &event);
	while (running)
	{
		std::memset(events, 0, sizeof(events));
		nb_events = epoll_wait(_epoll_fd, events, 100, 2000);
		if (nb_events == -1)
			derror("Epoll_wait failed");
		else if (nb_events > 0)
		{
			std::cout << "here" << std::endl;
			re = read(events[0].data.fd, buffer, 5);
			if (re > 0)
			 write(1, buffer, re);
			else if (re == 0)
			{
			 	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fdp[0], NULL);
				std::cout << "nothing" << std::endl;
			}
			else
				perror("coucou");
		}

	}
	return (0);
}