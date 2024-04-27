/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:29:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/27 10:19:39 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_sigint_received;

/*
	@brief	main is only used to setup the main data struct
			it doesn't actually do anything beyond that
*/
int	main(int argc, char **argv, char **envp)
{
	t_data			sh;
	int				i;

	if (DEBUG == 0)
		print_logo();
	sh.exit_flag = 0;
	sh.exit_status = 0;
	sh.envcp = ft_strarrdup(envp);
	if (!sh.envcp)
		return (printf("Error: failed to copy environment\n"), 1);
	sh.gc = gc_create();
	g_sigint_received = 0;
	setup_sigs();
	run_input_loop(&sh);
	i = -1;
	while (sh.envcp && sh.envcp[++i])
		free(sh.envcp[i]);
	if (sh.envcp)
		free(sh.envcp);
	(void)argc;
	(void)argv;
	return (sh.exit_status);
}
