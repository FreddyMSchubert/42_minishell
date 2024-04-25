/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 18:40:18 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	logical_and(t_bin_tree_node *node, t_program_data *program_data)
{
	t_pid_list	*pid_list;

	pid_list = NULL;
	if (node->l != NULL)
		program_data->exit_status = execute(node->l, program_data, &pid_list);
	wait_and_free(program_data, &pid_list);
	if (program_data->exit_status == 0 && node->r != NULL)
		program_data->exit_status = execute(node->r, program_data, &program_data->pid_list);
	return (program_data->exit_status);
}

int	logical_or(t_bin_tree_node *node, t_program_data *program_data)
{
	t_pid_list	*pid_list;

	pid_list = NULL;
	if (node->l != NULL)
		program_data->exit_status = execute(node->l, program_data, &pid_list);
	wait_and_free(program_data, &pid_list);
	if (program_data->exit_status != 0 && node->r != NULL)
		program_data->exit_status = execute(node->r, program_data, &program_data->pid_list);
	return (program_data->exit_status);
}
