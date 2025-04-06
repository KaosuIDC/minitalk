/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: sudelory <sudelory@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/18 13:07:56 by sudelory          #+#    #+#             */
/*   Updated: 2025/04/02 15:59:23 by sudelory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

static int	ft_atoi(const char *str)
{
	int	i;
	int	rev;
	int	result;

	i = 0;
	rev = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			rev *= -1;
		i++;
	}
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (rev * result);
}

static void	send_char(int pid, unsigned char character)
{
	int	bit;

	bit = 8;
	while (bit > 0)
	{
		bit--;
		if ((character >> bit) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(42);
		while (!g_confirm_process)
			;
		g_confirm_process = 0;
	}
}

void	send_signal(int pid, const char *str)
{
	int				i;
	unsigned char	len;

	len = (unsigned char)ft_strlen(str);
	send_char(pid, len);
	i = 0;
	while (str[i])
	{
		send_char(pid, str[i]);
		i++;
	}
	i = 0;
	while (i < 8)
	{
		kill(pid, SIGUSR2);
		usleep(42);
		while (!g_confirm_process)
			;
		g_confirm_process = 0;
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_id;

	if (argc == 3)
	{
		server_id = ft_atoi(argv[1]);
		signal(SIGUSR1, wait_for_process);
		send_signal(server_id, argv[2]);
	}
	else
		write(2, "Usage : Program name, Server Pid, Text to send.\n", 49);
	return (0);
}
