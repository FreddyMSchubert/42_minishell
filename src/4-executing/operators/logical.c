/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 12:25:26 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	logical_and(t_node *node, t_data *program_data)
{
	t_pid_list	*pid_list;

	pid_list = NULL;
	if (node->l != NULL)
		program_data->exit_status = execute(node->l, program_data, &pid_list);
	resolve_pid_list(program_data, &pid_list);
	if (program_data->exit_status == 0 && node->r != NULL)
		program_data->exit_status = \
					execute(node->r, program_data, &program_data->pid_list);
	return (program_data->exit_status);
}

int	logical_or(t_node *node, t_data *program_data)
{
	t_pid_list	*pid_list;

	pid_list = NULL;
	if (node->l != NULL)
		program_data->exit_status = execute(node->l, program_data, &pid_list);
	resolve_pid_list(program_data, &pid_list);
	if (program_data->exit_status != 0 && node->r != NULL)
		program_data->exit_status = \
					execute(node->r, program_data, &program_data->pid_list);
	return (program_data->exit_status);
}
