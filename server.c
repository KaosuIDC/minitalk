/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sudelory <sudelory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:07:51 by sudelory          #+#    #+#             */
/*   Updated: 2025/04/02 17:52:04 by sudelory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>

static void	ft_putpid(pid_t n)
{
	char	c;

	if (n > 9)
	{
		ft_putpid((n / 10));
		ft_putpid((n % 10));
	}
	else
	{
		c = 48 + n;
		write(1, &c, 1);
	}
}

static void	signal_handler(int signal, siginfo_t *info, void *context)
{
	static char	chr;
	static int	i;

	(void)context;
	if (i == 0)
		chr = 0;
	chr = chr << 1;
	if (signal == SIGUSR1)
		chr = chr | 1;
	i++;
	if (i == 8)
	{
		usleep(142);
		if (!chr)
			write(1, "\n", 1);
		else
			write(1, &chr, 1);
		chr = 0;
		i = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	pid_t				server_id;
	struct sigaction	sa;

	server_id = getpid();
	if (server_id < 0)
	{
		write(2, "Bad server id\n", 15);
		return (1);
	}
	ft_putpid(server_id);
	write(1, "\n", 1);
	sa.sa_sigaction = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
	{
		pause();
	}
	return (0);
}
