/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:54:08 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/04 18:20:48 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_exit(t_token **tokens, t_program_data *program_data, int out_fd)
{
	if (tokens[1])
		program_data->exit_status = ft_atoi(tokens[1]->value);
	else
		program_data->exit_status = 0;
	program_data->exit_flag = 1;
	gc_cleanup(program_data->gc);
	program_data->gc = create_garbage_collector();
	ft_putstr_fd("exit\n", out_fd);
	// if (out_fd != STDOUT_FILENO)
	// 	close(out_fd);
	return(program_data->exit_status);
}
