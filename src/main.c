/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:29:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/26 05:29:20 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_sigint_received;

int	main(int argc, char **argv, char **envp)
{
	t_program_data	program_data;
	int				i;

	program_data.exit_flag = 0;
	program_data.exit_status = 0;
	program_data.envcp = ft_strarrdup(envp);
	if (!program_data.envcp)
		return (ft_printf("Error: failed to copy environment\n"), 1);
	program_data.gc = create_garbage_collector();
	g_sigint_received = 0;
	setup_signals();
	run_crash_interface(&program_data);
	i = -1;
	while (program_data.envcp && program_data.envcp[++i])
		free(program_data.envcp[i]);
	if (program_data.envcp)
		free(program_data.envcp);
	(void)argc;
	(void)argv;
	return (0);
}
