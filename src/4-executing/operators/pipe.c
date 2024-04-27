/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:20:09 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 22:15:37 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	setup_pipe(t_node *node)
{
	int			pipe_fd[2];
	t_node		*input;
	t_node		*output;

	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("pipe error\n", 2);
		return ;
	}
	input = node->l;
	while (input->r != NULL)
	{
		if (input->val[0]->type == TOK_REDIR)
			break ;
		input = input->r;
	}
	output = node->r;
	while (output->l != NULL)
	{
		if (output->val[0]->type == TOK_REDIR)
			break ;
		output = output->l;
	}
	input->out_fd = pipe_fd[1];
	output->in_fd = pipe_fd[0];
}
