/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 09:24:51 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	logical_and(t_bin_tree_node *node, t_program_data *program_data)
{
	int	pid;
	int	status;

	pid = -1; // this will never occur just to silence warning
	if (node->l != NULL)
		pid = execute(node->l, program_data);
	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			program_data->exit_status = WEXITSTATUS(status);
	}
	if (program_data->exit_status == 0 && node->r != NULL)
		pid = execute(node->r, program_data);
	return (pid);
}

int	logical_or(t_bin_tree_node *node, t_program_data *program_data)
{
	int	pid;
	int	status;

	pid = 0; // this will never occur just to silence warning
	if (node->l != NULL)
		pid = execute(node->l, program_data);
	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			program_data->exit_status = WEXITSTATUS(status);
	}
	if (program_data->exit_status != 0 && node->r != NULL)
		pid = execute(node->r, program_data);
	return (pid);
}
