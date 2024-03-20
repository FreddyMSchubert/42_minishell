/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:15:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/20 08:58:39 by fschuber         ###   ########.fr       */
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
	// struct termios		terminal_settings;
	struct sigaction	signal_action;

/*
	if (isatty (STDIN_FILENO) != 0)
	{
		if (tcgetattr(STDIN_FILENO, &terminal_settings) == -1)
			return (-1); // handle error
		terminal_settings.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &terminal_settings) == -1)
			return (-1); // handle error
	}*/
	ft_memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = &sigint_handler;
	if (sigaction(SIGINT, &signal_action, NULL) == -1)
		return (-1); // handle error
	signal(SIGQUIT, SIG_IGN);
	return (0);
}