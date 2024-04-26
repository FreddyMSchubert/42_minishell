/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   should_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 10:45:40 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:13:55 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @brief checks if theres a heredoc before the current token
/// @return true if theres no heredoc before the current token, otherwise false
static bool	check_for_heredoc(const char *str, int i)
{
	int		j;

	j = i;
	while (j > 0 && !is_operator_symbol(str[j], ' ') && !ft_isspace(str[j]))
		j--;
	while (j > 0 && ft_isspace(str[j]))
		j--;
	if (j > 0 && ft_strnstr(&str[j - 1], "<<", 2) && !in_quote(str, '\"', \
		&str[j - 1]) && !in_quote(str, '\'', &str[j - 1]))
		return (false);
	return (true);
}

/// @brief checks if the string should be expanded
bool	should_expand(const char *str, int i, char exp_type)
{
	if (!check_for_heredoc(str, i))
		return (false);
	if (exp_type == '~' && (ft_isspace(str[i + 1]) || !str[i + 1] || str[i + 1] \
	== '/') && (i == 0 || ft_isspace(str[i - 1]) || is_operator_symbol(str \
	[i - 1], ' ')) && !in_quote(str, '\'', &str[i]) && !in_quote(str, '\"', \
	&str[i]) && !in_quote(str, '`', &str[i]) && !in_quote(str, '$', &str[i]) && \
	!in_quote(str, '\'', &str[i]) && !in_quote(str, '"', &str[i]))
		return (true);
	else if (exp_type == '?' && ft_strnstr(&str[i], "$?", 2) != NULL && \
			!in_quote(str, '\'', &str[i]))
		return (true);
	else if (exp_type == '"' && str[i] == '$' && \
			!in_quote(str, '\"', &str[i]) && !in_quote(str, '\'', \
			&str[i]) && (str[i + 1] == '\"' || str[i + 1] == '\''))
		return (true);
	else if (exp_type == '$' && is_valid_variable(&str[i + 1]) && \
			!in_quote(str, '\'', &str[i]))
		return (true);
	else if (exp_type == 's' && !is_valid_variable(&str[i + 1]) && \
			!in_quote(str, '\'', &str[i]) && !in_quote(str, '\"', &str[i]))
		return (true);
	else if (exp_type == '*' && !in_quote(str, '\'', \
			&str[i]) && !in_quote(str, '\"', &str[i]))
		return (true);
	return (false);
}
