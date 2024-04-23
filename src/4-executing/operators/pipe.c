/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:20:09 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/23 19:09:47 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	setup_pipe(t_bin_tree_node *node, t_program_data *program_data)
{
	int					pipe_fd[2];
	t_bin_tree_node		*current;

	node->l->input_fd = node->input_fd;
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("pipe error\n", 2);
		return ;
	}
	current = node->l;
	while (current->r != NULL)
	{
		if (current->val[0]->type == TOK_REDIR)
			break ;
		current = current->r;
	}
	current->output_fd = pipe_fd[1];
	node->r->input_fd = pipe_fd[0];
	(void)program_data;
}
