/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:29:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 12:26:33 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_program_data program_data;

	program_data.exit_flag = 0;
	program_data.envcp = ft_strarrdup(envp);
	program_data.gc = createGarbageCollector();

	run_crash_interface(&program_data);

	// silence unused variable warnings
	(void)argc;
	(void)argv;

	return (0);
}
