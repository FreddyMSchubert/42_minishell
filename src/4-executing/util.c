/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 08:37:07 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:18:44 by fschuber         ###   ########.fr       */
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
