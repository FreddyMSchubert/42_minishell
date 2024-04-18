/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:54:08 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/18 10:08:36 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_exit(t_token **tokens, t_program_data *program_data, int out_fd)
{
	if (tokens [1] && tokens[2])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		program_data->exit_status = 1;
		return (1);
	}
	if (tokens[1])
	{
		program_data->exit_status = ft_atoi(tokens[1]->value);
		program_data->exit_status %= 256;
		if (program_data->exit_status < 0)
			program_data->exit_status += 256;
	}
	else
		program_data->exit_status = 0;
	program_data->exit_flag = 1;
	if (isatty(fileno(stdin)))
		ft_putstr_fd("exit\n", out_fd);
	return(0);
}
