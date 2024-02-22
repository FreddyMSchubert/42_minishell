/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:29:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:56:50 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_program_data	program_data;
	int				i;

	program_data.exit_flag = 0;
	program_data.exit_status = 0;
	program_data.envcp = ft_strarrdup(envp);
	program_data.gc = create_garbage_collector();
	run_crash_interface(&program_data);
	i = 0;
	while (program_data.envcp[i])
	{
		free(program_data.envcp[i]);
		i++;
	}
	free(program_data.envcp);
	(void)argc;
	(void)argv;
	return (0);
}
