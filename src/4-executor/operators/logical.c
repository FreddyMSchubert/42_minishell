/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:32:19 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:58:22 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	logical_and(t_node *node, t_data *sh, t_pid_list **pid_list)
{
	t_pid_list	*local_pid_list;

	local_pid_list = NULL;
	if (node->l != NULL)
		sh->exit_status = execute(node->l, sh, &local_pid_list);
	resolve_pid_list(sh, &local_pid_list);
	if (sh->exit_status == 0 && node->r != NULL)
		sh->exit_status = \
					execute(node->r, sh, pid_list);
	return (sh->exit_status);
}

int	logical_or(t_node *node, t_data *sh, t_pid_list **pid_list)
{
	t_pid_list	*local_pid_list;

	local_pid_list = NULL;
	if (node->l != NULL)
		sh->exit_status = execute(node->l, sh, &local_pid_list);
	resolve_pid_list(sh, &local_pid_list);
	if (sh->exit_status != 0 && node->r != NULL)
		sh->exit_status = \
					execute(node->r, sh, pid_list);
	return (sh->exit_status);
}
