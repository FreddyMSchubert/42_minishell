/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/15 07:34:42 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// check end of tree null

// int	logical_op(t_bin_tree_node *node, t_program_data *program_data)
// {
// 	if (ft_strncmp(node->val[0]->value, "&&", 2) == 0)
// 		return (logical_and(node, program_data));
// 	if (ft_strncmp(node->val[0]->value, "||", 2) == 0)
// 		return (logical_or(node, program_data));
// 	return (-1);
// }

int	logical_and(t_bin_tree_node *node, t_program_data *program_data)
{
	int	pid;

	pid = -1; // this will never occur just to silence warning
	if (node->l != NULL)
		pid = execute(node->l, program_data);
	if (program_data->exit_status == 0 && node->r != NULL)
		pid = execute(node->r, program_data);
	return (pid);
}

int	logical_or(t_bin_tree_node *node, t_program_data *program_data)
{
	int	pid;

	pid = 0; // this will never occur just to silence warning
	if (node->l != NULL)
		pid = execute(node->l, program_data);
	if (program_data->exit_status != 0 && node->r != NULL)
		pid = execute(node->r, program_data);
	return (pid);
}
