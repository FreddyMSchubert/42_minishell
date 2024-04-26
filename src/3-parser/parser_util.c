/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:26 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 09:41:04 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Returns a pointer to a new token t_list, containing the tokens
		from start to end.
	Does not free or change the original token t_list.
	e.g. get_sub_token_arr({a, e, b, w, t, c, d}, 1, 3) -> {e, b, w}
*/
t_list	*sub_list(t_list *tokens, int start, int end, \
							t_data *program_data)
{
	int		index;
	t_list	*sub_token_list;
	t_list	*temp;

	if (start > end)
		return (NULL);
	index = -1;
	sub_token_list = NULL;
	while (tokens != NULL && ++index <= end)
	{
		if (index >= start)
		{
			temp = ft_lstnew(tokens->content);
			if (!temp)
				return (NULL);
			gc_append_element(program_data->gc, temp);
			if (!sub_token_list)
				sub_token_list = temp;
			else
				ft_lstadd_back(&sub_token_list, temp);
		}
		tokens = tokens->next;
	}
	return (sub_token_list);
}

int	toklen(t_list *tokens)
{
	int		counter;
	t_list	*tok;

	counter = 0;
	tok = tokens;
	while (tok != NULL)
	{
		counter++;
		tok = tok->next;
	}
	return (counter);
}

t_tok	*get_token_at_index(t_list *tokens, int index)
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

t_tok	**t_list_to_token_arr(t_list	*tokens, t_data	*program_data)
{
	t_tok	**token_arr;
	t_list	*temp;
	int		counter;

	counter = 0;
	temp = tokens;
	while (temp)
	{
		counter++;
		temp = temp->next;
	}
	token_arr = malloc(sizeof(t_tok *) * (counter + 1));
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
