/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substring_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:54:40 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:50:06 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	cuts off one set of braces from the token list
	@param	before_last_close_brace	pointer to the token list node
									before the last close brace
	@param	curr	pointer to the token list node
	@param	depth	depth of the braces
	@return	pointer to the first token after the last brace
			NULL	if there are no braces
*/
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

t_list	*update_depth_and_prev(t_list *curr, t_list *prev, int *depth)
{
	if (((t_tok *)curr->content)->type == TOK_OPEN_BRACE)
		(*depth)++;
	else if (((t_tok *)curr->content)->type == TOK_CLOSE_BRACE)
	{
		(*depth)--;
		if (prev)
			return (prev);
	}
	return (NULL);
}

t_list	*iterate_through_tokens(t_list *tokens, int *depth)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*before_last_close_brace;

	before_last_close_brace = NULL;
	curr = tokens;
	while (curr != NULL)
	{
		before_last_close_brace = update_depth_and_prev(curr, prev, depth);
		if (*depth == 0 && curr->next != NULL)
		{
			*depth = -1;
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
	return (before_last_close_brace);
}

/*
	@brief	checks if the token list is a substring
	@param	tokens	pointer to the token list
	@return	pointer to the first token after the last brace
			NULL	if there are no braces
*/
t_list	*check_substring(t_list *tokens)
{
	int		depth;
	t_list	*before_last_close_brace;

	depth = 0;
	before_last_close_brace = iterate_through_tokens(tokens, &depth);
	return (finish_substring(before_last_close_brace, tokens, depth));
}
