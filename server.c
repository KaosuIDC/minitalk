/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: sudelory <sudelory@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/18 13:07:51 by sudelory          #+#    #+#             */
/*   Updated: 2025/04/02 17:52:04 by sudelory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	check_state(siginfo_t *info)
{
	static int	last_client_pid;

	if (info->si_pid != last_client_pid)
	{
		last_client_pid = info->si_pid;
		return (1);
	}
	return (0);
}

static void	reset_state(char **buffer, int *buf_size, int *buf_index,
		char *chr)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	*buf_size = 0;
	*buf_index = 0;
	*chr = 0;
}

static void	process_char(char **buffer, int *buf_size, int *buf_index,
		char chr)
{
	if (*buf_size == 0)
	{
		*buf_size = (unsigned char)chr;
		*buffer = malloc(*buf_size + 1);
		if (!*buffer)
		{
			write(2, "Malloc fail\n", 12);
			exit(1);
		}
		*buf_index = 0;
	}
	else if (*buf_index < *buf_size)
		(*buffer)[(*buf_index)++] = chr;
	if (*buf_index == *buf_size)
	{
		(*buffer)[*buf_index] = '\0';
		write(1, *buffer, *buf_index);
		free(*buffer);
		*buffer = NULL;
		*buf_index = 0;
		*buf_size = 0;
	}
}

static void	signal_handler(int signal, siginfo_t *info, void *context)
{
	static char	chr;
	static int	i;
	static char	*buffer;
	static int	buf_size;
	static int	buf_index;

	(void)context;
	if (check_state(info) == 1)
	{
		reset_state(&buffer, &buf_size, &buf_index, &chr);
		i = 0;
	}
	chr = (chr << 1) | (signal == SIGUSR1);
	if (++i == 8)
	{
		process_char(&buffer, &buf_size, &buf_index, chr);
		i = 0;
	}
	if (info->si_pid > 0)
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
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (1);
	sigaction(SIGUSR2, &sa, NULL);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (1);
	while (1)
		pause();
	return (0);
}
