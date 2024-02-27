/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:20:09 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/27 17:00:50 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	setup_pipe(t_bin_tree_node *node, t_program_data *program_data)
{
	int		pipe_fd[2];
	pid_t	pid;

	// printf("setting up pipe\n");
	node->l->input_fd = node->input_fd;
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("pipe error\n", 2);
		return ;
	}
	node->l->output_fd = pipe_fd[1];
	node->r->input_fd = pipe_fd[0];
	// printf("fds in pipe: %d %d\n", pipe_fd[1], pipe_fd[0]);
	(void)program_data;
}
