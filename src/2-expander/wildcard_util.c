/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:11:47 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/24 10:30:48 by niklasburch      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_rid_of_quotes_wildcard(char *str)
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
			new_str[j++] = '\\'; // Escape the * character
		if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	// printf("new_str: .%s.\n", new_str);
	return (new_str);
}

char	*get_pattern(char *str, int index, t_data *program_data)
{
	int	i;
	char	*ret;

	i = index;
	while (str[i] && ((str[i] != ' ' && !is_operator_symbol(str[i], ' ')) || is_in_quote(str, '"', &str[i]) || is_in_quote(str, '\'', &str[i])))
		i++;
	while (index > 0 && ((str[index - 1] != ' ' && !is_operator_symbol(str[index - 1], ' ')) || is_in_quote(str, '"', &str[index - 1]) || is_in_quote(str, '\'', &str[index - 1])))
		index--;
	ret = ft_substr(&str[index], 0, i - index);
	gc_append_element(program_data->gc, ret);
	// printf("pattern: .%s.\n", ret);
	return (get_rid_of_quotes_wildcard(ret));
}