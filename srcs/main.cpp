/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 11:59:37 by lgiband           #+#    #+#             */
/*   Updated: 2022/11/23 12:03:25 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Configure.hpp"

int	main(void)
{
	Configure config("test_config");

	if (!config.isGood())
		return (1);
	
	WebServer server(config);
	server.init();
	
	return (0);
}