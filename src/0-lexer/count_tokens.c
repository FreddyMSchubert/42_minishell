/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:41:21 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 17:30:21 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_quotes(const char *s, int *i, int *in_quote)
{
	if ((s[*i] == '"' && *in_quote != 2) || (s[*i] == '\'' && *in_quote != 1))
	{
		if (*in_quote == 0)
		{
			if (s[*i] != '\"')
				*in_quote = 2;
			else
				*in_quote = 1;
		}
		else
			*in_quote = 0;
	}
	return (0);
}

int	handle_operator_symbols(const char *s, int *i, int *count, int *in_word)
{
	if (is_operator_symbol(s[*i], s[*i + 1]))
	{
		if (is_operator_symbol(s[*i], s[*i + 1]) == 2)
			(*i)++;
		(*count)++;
		*in_word = 0;
	}
	return (0);
}

int	count_tokens(const char *s)
{
	int	i;
	int	count;
	int	in_word;
	int	in_quote;

	i = -1;
	count = 0;
	in_word = 0;
	in_quote = 0;
	while ((int)ft_strlen(s) > i && s[++i])
	{
		handle_quotes(s, &i, &in_quote);
		handle_operator_symbols(s, &i, &count, &in_word);
		if (!ft_isspace(s[i]) && !in_word && !in_quote)
		{
			count++;
			in_word = 1;
		}
		else if (ft_isspace(s[i]) && !in_quote)
			in_word = 0;
	}
	return (count);
}
