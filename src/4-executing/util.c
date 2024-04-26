/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 08:37:07 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 18:25:52 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	closes the file descriptors of a node
*/
void	close_fds(t_node *node)
{
	if (node->in_fd != STDIN_FILENO)
		close(node->in_fd);
	if (node->out_fd != STDOUT_FILENO)
		close(node->out_fd);
}

void	child_process_exit(t_data *sh, int exitcode)
{
	int			i;

	gc_cleanup(sh->gc);
	i = -1;
	while (sh->envcp[++i])
		free(sh->envcp[i]);
	free(sh->envcp);
	exit(exitcode);
}
