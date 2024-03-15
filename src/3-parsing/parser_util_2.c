/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:02:14 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/15 10:52:33 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*get_token_at_index(t_list *tokens, int index)
{
	int		counter;
	t_list	*tok;

	counter = 0;
	tok = tokens;
	while (tok != NULL)
	{
		if (counter == index)
			return (tok->content);
		counter++;
		tok = tok->next;
	}
	return (NULL);
}

t_token	**t_list_to_token_arr(t_list	*tokens, t_program_data	*program_data)
{
	t_token	**token_arr;
	t_list	*temp;
	int		counter;

	counter = 0;
	temp = tokens;
	while (temp)
	{
		counter++;
		temp = temp->next;
	}
	token_arr = malloc(sizeof(t_token *) * (counter + 1));
	if (!token_arr)
		return (NULL);
	gc_append_element(program_data->gc, token_arr);
	token_arr[counter] = NULL;
	counter = 0;
	temp = tokens;
	while (temp)
	{
		token_arr[counter] = temp->content;
		counter++;
		temp = temp->next;
	}
	return (token_arr);
}
