/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 09:55:57 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/18 09:59:33 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	child_process_exit(t_program_data *program_data, int exitcode)
{
	int			i;

	gc_cleanup(program_data->gc);
	free(program_data->gc);
	i = -1;
	while (program_data->envcp[++i])
		free(program_data->envcp[i]);
	free(program_data->envcp);
	exit(exitcode);
}
