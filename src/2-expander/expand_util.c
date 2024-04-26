/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:07:07 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 12:13:00 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	find_closing_quote(const char *str, int *i)
{
	int		j;
	char	quote;

	j = 1;
	quote = str[0];
	while (str[j] && str[j] != quote)
		j++;
	*i += j;
	return (j);
}

char	*quote_operators(char *envcp_value)
{
	int		i;
	int		j;
	char	*new_str;
	int		is_op;

	i = -1;
	j = 0;
	new_str = ft_calloc(ft_strlen(envcp_value) * 2 + 1, sizeof(char));
	if (!new_str)
		return (free(envcp_value), NULL);
	while (envcp_value[++i])
	{
		is_op = is_operator_symbol(envcp_value[i], envcp_value[i + 1]);
		if (is_op)
		{
			new_str[j++] = '"';
			new_str[j++] = envcp_value[i];
			if (is_op == 2)
				new_str[j++] = envcp_value[++i];
			new_str[j++] = '"';
		}
		else
			new_str[j++] = envcp_value[i];
	}
	return (new_str);
}

bool	in_quote(const char *str, char quote, const char *current_char)
{
	int		i;
	bool	d_quote;
	bool	s_quote;

	i = -1;
	d_quote = false;
	s_quote = false;
	if (!quote || !str || !current_char)
		return (false);
	while (&str[i] != current_char && str[++i])
	{
		if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
	}
	if (quote == '\"')
		return (d_quote);
	else
		return (s_quote);
}

char	*get_rid_of_quotes(char *str)
{
	int		i;
	int		j;
	char	*new_str;
	bool	s_quote;
	bool	d_quote;

	s_quote = false;
	d_quote = false;
	i = -1;
	j = 0;
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	while (str[++i])
	{
		if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	return (new_str);
}
