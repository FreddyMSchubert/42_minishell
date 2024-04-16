/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 09:55:57 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/08 17:27:52 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	child_process_exit(t_program_data *program_data, int exitcode)
{
	int			i;

	gc_cleanup(program_data->gc);
	i = -1;
	while (program_data->envcp[++i])
		free(program_data->envcp[i]);
	free(program_data->envcp);
	exit(exitcode);
}
