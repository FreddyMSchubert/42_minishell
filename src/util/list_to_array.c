/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 09:49:34 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/14 10:03:25 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	**list_to_token_array(t_list *list)
{
	t_token	**array;
	int		i;
	t_list	*temp;

	temp = list;
	i = 0;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	array = malloc(sizeof(t_token *) * (i + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (list)
	{
		array[i] = ((t_token *)list->content);
		list = list->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}
