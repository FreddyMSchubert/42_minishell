/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:18:59 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 06:08:53 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
	Garbage Collector has one dummy node at the start just as a reference.
*/

t_list	*gc_create(void)
{
	t_list	*gc;

	gc = malloc(sizeof(t_list));
	if (!gc)
		return (NULL);
	gc->content = NULL;
	gc->next = NULL;
	return (gc);
}

int	gc_append_element(t_list *gc, void *content)
{
	t_list	*new_node;
	t_list	*selected_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (-1);
	new_node->content = content;
	new_node->next = NULL;
	selected_node = gc;
	while (selected_node->next != NULL)
	{
		if (selected_node->content == content)
			return (free(new_node), -2);	// duplicated pointer
		selected_node = selected_node->next;
	}
	selected_node->next = new_node;
	return (0);
}

// frees all elements in the garbage collector
// after this, the garbage collector is freed, so it should be reinitialized
void	gc_cleanup(t_list *gc)
{
	if (VERBOSE == 1)
		printf("cleaning up garbage collector\n");
	ft_lstclear(&gc, free);
}
