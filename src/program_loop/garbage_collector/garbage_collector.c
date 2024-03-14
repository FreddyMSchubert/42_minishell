/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:18:59 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/14 09:57:05 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
	Garbage Collector has one dummy node at the start just as a reference.
*/

t_list	*create_garbage_collector(void)
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
		selected_node = selected_node->next;
	selected_node->next = new_node;
	return (0);
}

void	gc_append_element_array(t_list *gc, void *content)
{
	void	**array;
	int		i;

	array = (void **) content;
	i = 0;
	while (array[i] != NULL)
	{
		gc_append_element(gc, array[i]);
		i++;
	}
	gc_append_element(gc, content);
}

void	gc_append_t_list(t_list *gc, t_list *linkedlist)
{
	t_list	*selected_node;

	selected_node = linkedlist;
	while (selected_node->next != NULL)
	{
		gc_append_element(gc, selected_node);
		selected_node = selected_node->next;
	}
	gc_append_element(gc, selected_node);
}

// frees all elements in the garbage collector
void	gc_cleanup(t_list *gc)
{
	t_list	*current;
	t_list	*current2;

	current = gc;
	while (current != NULL)
	{
		current2 = current;
		current = current->next;
		free(current2->content);
		free(current2);
	}
}
