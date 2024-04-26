/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 06:54:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 18:29:59 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_list	*create_tokens(char **split_input, t_data *data)
{
	int		counter;
	int		token_amount;
	int		is_first_or_after_operator;
	t_list	*tokens;

	token_amount = 0;
	while (split_input && split_input[token_amount])
		token_amount++;
	counter = 0;
	tokens = NULL;
	while (counter < token_amount)
	{
		is_first_or_after_operator = 0;
		if (counter == 0 || (is_operator_symbol(split_input[counter - 1][0], \
				split_input[counter - 1][1]) != 0 && \
				!is_redirect(*split_input[counter - 1])))
			is_first_or_after_operator = 1;
		ft_lstadd_back(&tokens, detect_token_type(split_input[counter], \
		is_first_or_after_operator, data));
		if (ft_lstlast(tokens)->content == NULL)
			return (NULL);
		counter++;
	}
	return (tokens);
}

t_list	*lexer(char *input, t_data *data)
{
	char	**split_input;

	split_input = ms_split(put_space_between_tokens(input, data), data);
	if (!split_input)
		return (NULL);
	return (create_tokens(split_input, data));
}
