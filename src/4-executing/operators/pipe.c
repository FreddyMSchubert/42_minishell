/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:20:09 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/23 19:57:04 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	setup_pipe(t_bin_tree_node *node, t_program_data *program_data)
{
	int					pipe_fd[2];
	t_bin_tree_node		*input;
	t_bin_tree_node		*output;

	node->l->input_fd = node->input_fd;
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
		ft_printf("\t\tcontinueing traversal, current node content %s\n", output->val[0]->value);
		if (output->val[0]->type == TOK_REDIR)
			break ;
		output = output->l;
	}
	input->output_fd = pipe_fd[1];
	output->input_fd = pipe_fd[0];
	ft_printf("creating pipe:\twrite: %s [%d],\tread: %s [%d]\n", input->val[0]->value, input->output_fd, output->val[0]->value, output->input_fd);
	(void)program_data;
}
