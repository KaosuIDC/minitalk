/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: sudelory <sudelory@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/04/04 17:54:51 by sudelory          #+#    #+#             */
/*   Updated: 2025/04/04 17:54:51 by sudelory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_confirm_process;

void		wait_for_process(int signal);
void		ft_putpid(pid_t n);

#endif