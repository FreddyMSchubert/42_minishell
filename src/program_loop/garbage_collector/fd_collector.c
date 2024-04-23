/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_collector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 07:25:39 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/23 08:21:39 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// -1 is used as a dummy value for the first node
t_fdlist	*create_fd_collector(void)
{
	t_fdlist	*fd_gc;

	fd_gc = malloc(sizeof(t_fdlist));
	if (!fd_gc)
		return (NULL);
	fd_gc->fd = -1;
	fd_gc->next = NULL;
	return (fd_gc);
}

// allows for appending of two fds at once. if you only need one pass -1
int	fd_collector_append(t_program_data *data, int fd1, int fd2)
{
	t_fdlist	*new_node1;
	t_fdlist	*new_node2;
	t_fdlist	*current;

	new_node1 = NULL;
	new_node2 = NULL;
	new_node1 = malloc(sizeof(t_fdlist));
	if (!new_node1)
	{
		if (fd1 != -1)
			close(fd1);
		if (fd2 != -1)
			close(fd2);
		exit_error("Failed to allocate memory for fd1", 1, data);
	}
	new_node1->fd = fd1;
	new_node1->next = NULL;

	if (fd2 != -1)
	{
		new_node2 = malloc(sizeof(t_fdlist));
		if (!new_node2)
		{
			free(new_node1);
			if (fd1 != -1)
				close(fd1);
			if (fd2 != -1)
				close(fd2);
			exit_error("Failed to allocate memory for fd2", 1, data);
		}
		new_node2->fd = fd2;
		new_node2->next = NULL;
	}

	current = data->fd_gc;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node1;

	if (new_node2)
		new_node1->next = new_node2;

	return (0);
}

// frees all file descriptors in the collector and the collector itself
void	fd_collector_cleanup(t_fdlist *fd_gc)
{
	t_fdlist	*current;
	t_fdlist	*next;

	if (VERBOSE == 1)
		printf("Cleaning up file descriptor collector\n");
	current = fd_gc;
	while (current != NULL)
	{
		next = current->next;
		if (current->fd != -1)
			close(current->fd);
		free(current);
		current = next;
	}
}
