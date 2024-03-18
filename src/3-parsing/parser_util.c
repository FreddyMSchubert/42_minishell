/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:26 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/18 07:54:13 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Returns a pointer to a new token t_list, containing the tokens
		from start to end.
	Does not free or change the original token t_list.
	e.g. get_sub_token_arr({a, e, b, w, t, c, d}, 1, 3) -> {e, b, w}
*/
t_list	*sub_token_t_list(t_list *tokens, int start, int end, t_program_data *program_data)
{
	int		index;
	t_list	*current_token;
	t_list	*sub_token_list;
	t_list	*temp;

	if (start > end)
		return (NULL);
	index = 0;
	current_token = tokens;
	sub_token_list = NULL;
	while (current_token != NULL && index <= end)
	{
		if (index >= start)
		{
			temp = ft_lstnew(current_token->content);
			if (!temp)
				return (NULL);
			gc_append_element(program_data->gc, temp);
			if (!sub_token_list)
				sub_token_list = temp;
			else
				ft_lstadd_back(&sub_token_list, temp);
		}
		current_token = current_token->next;
		index++;
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

// returns -42 if there isnt a non-ignored token
int	first_non_ignored(t_list *tokens)
{
	int		counter;
	t_list	*current;

	counter = 0;
	current = tokens;
	while (current != NULL && ((t_token *)current->content)->ignored == 1)
	{
		counter++;
		current = current->next;
	}
	if (current == NULL)
		return (-42);
	return (counter);
}

// returns -42 if there isnt a non-ignored token
int	last_non_ignored(t_list *tokens)
{
	t_list	*current;
	int		counter;
	int		last_non_ignored;

	counter = 0;
	current = tokens;
	last_non_ignored = -42;
	while (current != NULL)
	{
		if (((t_token *)current->content)->ignored == 0)
			last_non_ignored = counter;
		counter++;
		current = current->next;
	}
	return (last_non_ignored);
}
