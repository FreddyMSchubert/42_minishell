/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substring_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:54:40 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 14:09:31 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_list	*finish_substring(t_list	*before_last_close_brace, \
									t_list	*curr, int depth)
{
	if (depth == 0)
	{
		if (before_last_close_brace)
			before_last_close_brace->next = NULL;
		if (curr && curr->next)
			return (curr->next);
	}
	return (NULL);
}

/*
	Checks whether the inputted token array is a substring
	If so, it returns the first token after the substring start and next nulls
	the last token before the substring end
	returns NULL if no substring is found
*/
t_list	*check_substring(t_list *tokens)
{
	int		depth;
	t_list	*curr;
	t_list	*prev;
	t_list	*before_last_close_brace;

	depth = 0;
	before_last_close_brace = NULL;
	curr = tokens;
	while (curr != NULL)
	{
		if (((t_tok *)curr->content)->type == TOK_OPEN_BRACE)
			depth++;
		else if (((t_tok *)curr->content)->type == TOK_CLOSE_BRACE)
		{
			depth--;
			if (prev)
				before_last_close_brace = prev;
		}
		if (depth == 0 && curr->next != NULL)
		{
			depth = -1;
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
	return (finish_substring(before_last_close_brace, tokens, depth));
}
