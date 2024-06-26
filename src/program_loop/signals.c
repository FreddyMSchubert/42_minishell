/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:15:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:04:18 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	signal handler for SIGINT
*/
static void	sigint_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	if (g_sigint_received == 0)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_sigint_received == 2)
		ioctl(0, TIOCSTI, "\4");
}

/*
	@brief	sets up signal handlers
*/
void	setup_sigs(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
