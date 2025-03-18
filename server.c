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
		if (chr == 0)
			write (1, "\n", 1);
		else
			write(1, &chr, 1);
		i = 0;
		chr = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	pid_t	server_id;
	struct sigaction sa;

	server_id = getpid();
	ft_putpid(server_id);
	write (1, "\n", 1);
	sa.sa_sigaction = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
	{
		pause();
	}
	return (0);
}