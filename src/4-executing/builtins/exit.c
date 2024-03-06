/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:54:08 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/28 14:42:25 by nburchha         ###   ########.fr       */
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
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(program_data->exit_status);
}
