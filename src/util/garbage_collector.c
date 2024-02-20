/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:18:59 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 13:05:58 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	Garbage Collector has one dummy node at the start just as a reference.
*/

/*
	Logs message & exits program with exit_code, 
	but not before freeing all elements in the garbage collector
*/
void exit_error(char *message, int exit_code, t_list* gc)
{
	ft_printf("%s\n", message);
	cleanup(gc);
	exit(exit_code);
}

t_list* createGarbageCollector()
{
	t_list* gc = malloc(sizeof(t_list));
	if (!gc)
		return (NULL);
	gc->content = NULL;
	gc->next = NULL;
	return gc;
}

int	appendElement(t_list* gc, void* content)
{
	t_list *newNode;
	t_list *selectedNode;

	newNode = malloc(sizeof(t_list));
	if (!newNode)
		return (-1);
	newNode->content = content;
	newNode->next = NULL;
	selectedNode = gc;
	while (selectedNode->next != NULL)
		selectedNode = selectedNode->next;
	selectedNode->next = newNode;
	return (0);
}

void appendElementArray(t_list* gc, void* content)
{
	void **array;
	int i;

	array = (void**)content;
	i = 0;
	while (array[i] != NULL)
	{
		appendElement(gc, array[i]);
		i++;
	}
	appendElement(gc, content);
}

// frees all elements in the garbage collector
void cleanup(t_list* gc)
{
	t_list *current;
	t_list *current2;

	current = gc;
	while (current != NULL)
	{
		current2 = current;
		current = current->next;
		free(current2->content);
		free(current2);
	}
}
