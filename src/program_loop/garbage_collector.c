/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:18:59 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/27 11:00:30 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Garbage Collector has one dummy node at the start just as a reference.
*/

/*
	Logs message & exits program with exit_code, 
	but not before freeing all elements in the garbage collector
*/
void	exit_error(char *message, int exit_code, t_list *gc)
{
	ft_printf("%s\n", message);
	cleanup(gc);
	exit(exit_code);
}

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

int	append_element(t_list *gc, void *content)
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

void	append_element_array(t_list *gc, void *content)
{
	void	**array;
	int		i;

	array = (void **) content;
	i = 0;
	while (array[i] != NULL)
	{
		append_element(gc, array[i]);
		i++;
	}
	append_element(gc, content);
}

// frees all elements in the garbage collector
void	cleanup(t_list *gc)
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
	clear_history();
}