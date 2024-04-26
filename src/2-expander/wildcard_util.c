/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:11:47 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:20:39 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '\"' && !*s_quote)
		*d_quote = !*d_quote;
}

/// @brief Get rid of quotes in the wildcard pattern and escape wildcard 
/// character if it is in quotes
static char	*get_rid_of_quotes_wildcard(char *str)
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
		if (str[i] == '*' && (s_quote || d_quote))
			new_str[j++] = '\\';
		process_quotes(str[i], &s_quote, &d_quote);
		if (str[i] != '\'' && str[i] != '\"')
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	return (new_str);
}

/// @brief Get the pattern of the wildcard
char	*get_pattern(const char *str, int index, t_data *data)
{
	int		i;
	char	*ret;

	i = index;
	while (str[i] && ((str[i] != ' ' && !is_operator_symbol(str[i], ' ')) || \
	in_quote(str, '"', &str[i]) || in_quote(str, '\'', &str[i])))
		i++;
	while (index > 0 && ((str[index - 1] != ' ' && \
	!is_operator_symbol(str[index - 1], ' ')) || in_quote(str, '"', &str[index \
	- 1]) || in_quote(str, '\'', &str[index - 1])))
		index--;
	ret = ft_substr(&str[index], 0, i - index);
	gc_append_element(data->gc, ret);
	ret = get_rid_of_quotes_wildcard(ret);
	gc_append_element(data->gc, ret);
	return (ret);
}
