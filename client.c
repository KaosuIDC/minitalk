#include <signal.h>
#include <unistd.h>

int	confirm_process = 0;

void wait_for_process(int signal)
{
	(void)signal;
	confirm_process = 1;
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

void	send_signal(int pid, unsigned char character)
{
	int	i;
	unsigned char	temp;
	i = 8;
	temp = character;
	while (i > 0)
	{
		i--;
		temp = character >> i;
		if (temp % 2 == 0)
			kill(pid, SIGUSR2);
		else
		 	kill(pid, SIGUSR1);
		usleep(42);
		while (!confirm_process)
			;
		confirm_process = 0;
	}
}

void	send_for_termination_signal(int pid)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		kill(pid, SIGUSR2);
		usleep(42);
		while (!confirm_process)
			;
		confirm_process = 0;
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_id;
	int		i;

	if (argc == 3)
	{
		server_id = ft_atoi(argv[1]);
		signal(SIGUSR1, wait_for_process);
		i = 0;
		while (argv[2][i])
		{
			send_signal(server_id, argv[2][i]);
			i++;
		}
		send_for_termination_signal(server_id);
	}
	else 
		write (2, "Bad number of arguments.\n", 25);
	return (0);
}
