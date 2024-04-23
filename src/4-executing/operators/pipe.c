/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:20:09 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/23 10:31:08 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	setup_pipe(t_bin_tree_node *node, t_program_data *program_data)
{
	int		pipe_fd[2];

	node->l->input_fd = node->input_fd;
	if (pipe(pipe_fd) == -1)
		exit_error("pipe error", 2, program_data);
	node->l->output_fd = pipe_fd[1];
	node->r->input_fd = pipe_fd[0];
	if (VERBOSE == 1)
		ft_printf("created pipe: write: %d, read: %d; left: %s, right: %s\n", \
					node->l->output_fd, node->r->input_fd);
	(void)program_data;
}
