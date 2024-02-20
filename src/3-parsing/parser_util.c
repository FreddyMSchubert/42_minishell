/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:26 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 08:39:24 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Returns a pointer to a new token array, containing the tokens from start to end.
	Does not free the original token array.
	e.g. get_sub_token_arr({a, e, b, w, t, c, d}, 1, 3) -> {e, b, w}
*/
t_token	**sub_tok_arr(t_token **token_arr, int start, int end)
{
	t_token			**sub_token_arr;
	int				counter;

	if (start > end)
		return (NULL);
	sub_token_arr = malloc(sizeof(t_token *) * (end - start + 1 + 1));
	if (!sub_token_arr)
		return (NULL);
	sub_token_arr[end - start + 1] = NULL;
	counter = 0;
	while (start <= end)
	{
		sub_token_arr[counter] = token_arr[start];
		counter++;
		start++;
	}
	return (sub_token_arr);
}

int	toklen(t_token **token_arr)
{
	int	counter;

	counter = 0;
	while (token_arr[counter])
		counter++;
	return (counter);
}
