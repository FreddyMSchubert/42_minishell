/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 09:49:21 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	logical_and(t_bin_tree_node *node, t_program_data *program_data, t_pid_list **pid_list)
{
	int	pid;
	int	status;

	pid = -1;
	if (node->l != NULL)
		pid = execute(node->l, program_data, pid_list);
	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			program_data->exit_status = WEXITSTATUS(status);
		pid = -1;
	}
	if (program_data->exit_status == 0 && node->r != NULL)
		pid = execute(node->r, program_data, pid_list);
	return (pid);
}

int	logical_or(t_bin_tree_node *node, t_program_data *program_data, t_pid_list **pid_list)
{
	int	pid;
	int	status;

	pid = -1;
	if (node->l != NULL)
		pid = execute(node->l, program_data, pid_list);
	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		pid = -1;
		if (WIFEXITED(status))
			program_data->exit_status = WEXITSTATUS(status);
	}
	if (program_data->exit_status != 0 && node->r != NULL)
		pid = execute(node->r, program_data, pid_list);
	return (pid);
}
