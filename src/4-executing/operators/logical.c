/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/01 16:11:52 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// check end of tree null

int	logical_op(t_bin_tree_node *node, t_program_data *program_data, t_list *pids)
{
	if (ft_strncmp(node->val[0]->value, "&&", 2) == 0)
		return (logical_and(node, program_data, pids));
	if (ft_strncmp(node->val[0]->value, "||", 2) == 0)
		return (logical_or(node, program_data, pids));
	return (-1);
}

int	logical_and(t_bin_tree_node *node, t_program_data *program_data, t_list *pids)
{
	int	status;

	status = 0; // this will never occur just to silence warning
	if (node->l != NULL)
		status = execute_node(node->l, program_data, pids);
	if (status == 0 && node->r != NULL)
		status = execute_node(node->r, program_data, pids);
	return (status);
}

int	logical_or(t_bin_tree_node *node, t_program_data *program_data, t_list *pids)
{
	int	status;

	status = 0; // this will never occur just to silence warning
	if (node->l != NULL)
		status = execute_node(node->l, program_data, pids);
	if (status != 0 && node->r != NULL)
		status = execute_node(node->r, program_data, pids);
	return (status);
}
