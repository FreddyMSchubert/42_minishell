/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:54:08 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/18 07:18:31 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_exit(t_token **tokens, t_program_data *program_data, int cmd_start_index)
{
	// return exit status of previous command error
	// if (tokens[cmd_start_index + 1] && tokens[cmd_start_index + 2])
	// 	program_data->exit_status = 255; // too many arguments
	if (tokens[cmd_start_index + 1])
		program_data->exit_status = ft_atoi(tokens[cmd_start_index + 1]->value);
	else
		program_data->exit_status = 0;
	program_data->exit_flag = 1;
	gc_cleanup(program_data->gc);
	program_data->gc = create_garbage_collector();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return(program_data->exit_status);
}
