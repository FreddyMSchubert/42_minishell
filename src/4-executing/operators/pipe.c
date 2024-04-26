/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:20:09 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/25 10:46:14 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	setup_pipe(t_node *node, t_data *program_data)
{
	int					pipe_fd[2];
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
	if (VERBOSE == 1)
		printf("creating pipe:\twrite: %s [%d],\tread: %s [%d]\n", input->val[0]->val, input->out_fd, output->val[0]->val, output->in_fd);
	(void)program_data;
}
