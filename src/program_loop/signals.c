/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:15:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 10:11:14 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint_received;

// static void	redisplay_prompt(void)
// {
// 	printf("\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

static void	sigint_handler(int signo)
{
	if (signo != SIGINT)
		return (g_sigint_received = 0, (void)0);
	g_sigint_received = SIGINT;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

int	setup_signals(void)
{
	struct sigaction	signal_action;

	ft_memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = &sigint_handler;
	if (sigaction(SIGINT, &signal_action, NULL) == -1)
		return (ft_putstr_fd("crash: sigs: error inserting sigint handler", STDERR_FILENO), -1);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
