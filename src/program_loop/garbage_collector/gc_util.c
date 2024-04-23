/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:43:39 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/23 08:36:21 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
	Logs message & exits program with exit_code,
	but not before freeing all elements in the garbage collector
*/
void	exit_error(char *msg, int exit_code, t_program_data *data)
{
	ft_putstr_fd("crash: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	gc_cleanup(data->gc);
	fd_collector_cleanup(data->fd_gc);
	exit(exit_code);
}

void	cleanup_gc_and_fd(t_program_data *program_data)
{
	gc_cleanup(program_data->gc);
	program_data->gc = create_garbage_collector();
	fd_collector_cleanup(program_data->fd_gc);
	program_data->fd_gc = create_fd_collector();
}
