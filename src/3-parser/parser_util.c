/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:26 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:33:26 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	creates a new token list containing the tokens from start to end
	@param	tokens	pointer to the token list
	@param	start	index of the first token to include
	@param	end		index of the last token to include
	@param	sh		pointer to the data struct
	@return	pointer to the new token list
			NULL	if malloc failed
	e.g. get_sub_token_arr({a, e, b, w, t, c, d}, 1, 3) -> {e, b, w}
*/
t_list	*sub_list(t_list *tokens, int start, int end, \
							t_data *sh)
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
			gc_append_element(sh->gc, temp);
			if (!sub_token_list)
				sub_token_list = temp;
			else
				ft_lstadd_back(&sub_token_list, temp);
		}
		tokens = tokens->next;
	}
	return (sub_token_list);
}

/*
	@brief	gets the length of a token list
	@param	tokens	pointer to the token list
	@return	length of the token list
*/
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

/*
	@brief	gets the token at a specific index
	@param	tokens	pointer to the token list
	@param	index	index of the token to get
	@return	pointer to the token
			NULL	if the index is out of bounds
*/
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

/*
	@brief	converts a token list to an array of tokens
	@param	tokens	pointer to the token list
	@param	sh	pointer to the data struct
	@return	pointer to the token array
			NULL	if malloc failed
*/
t_tok	**t_list_to_token_arr(t_list	*tokens, t_data	*sh)
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
	gc_append_element(sh->gc, token_arr);
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
