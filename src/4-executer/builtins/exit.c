/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:54:08 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:01:25 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_exit(t_token **tokens, t_program_data *program_data)
{
	program_data->exit_flag = 1;
	if (tokens[1] != NULL)
		program_data->exit_status = 255;
	// return exit status of previous command error
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (program_data->exit_status);
}
