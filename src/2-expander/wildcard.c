/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:44:01 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:32:26 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @brief concatenates two strings, adding single quotes before and after the
/// string, freeing the first one
static char	*concatenate_matches_free_s1(char *s1, const char *s2)
{
	char	*new_str;
	size_t	s1_len;
	size_t	s2_len;

	if (!s2)
		return (NULL);
	if (!s1)
		s1_len = 0;
	else
		s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_str = malloc(s1_len + s2_len + 4);
	if (!new_str)
		return (NULL);
	if (s1)
	{
		ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
		free(s1);
	}
	new_str[s1_len] = '\'';
	ft_strlcpy(new_str + s1_len + 1, s2, ft_strlen(s2) + 1);
	new_str[s1_len + s2_len + 1] = '\'';
	new_str[s1_len + s2_len + 2] = ' ';
	new_str[s1_len + s2_len + 3] = '\0';
	return (new_str);
}

/// @brief processes the '*' character in a pattern.
static int	process_star(const char **p, const char **f, const char **star, \
				const char **sf)
{
	if (*(*p) == '*' && (*p == *star || *(*p - 1) != '\\'))
	{
		*star = (*p)++;
		*sf = *f;
	}
	else if (*star)
	{
		*p = *star + 1;
		*f = ++(*sf);
	}
	else
		return (0);
	return (1);
}

/// @brief matches a pattern with a filename
int	match(const char *pattern, const char *filename)
{
	const char	*p = pattern;
	const char	*f = filename;
	const char	*star = NULL;
	const char	*sf = NULL;

	while (*f)
	{
		if (star && !(*star + 1))
			return (1);
		if (*p == *f && *p != '*' && p++ && f++)
			;
		else if (*p == '\\' && *(p + 1) == '*' && *f == '*' && *f++)
			p += 2;
		else if (!process_star(&p, &f, &star, &sf))
			return (0);
	}
	while (*p == '*' && (p == pattern || *(p - 1) != '\\'))
		p++;
	return (*p == '\0');
}

/// @brief lists all the files that match the pattern
static char	*list_matching_files(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*result;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir() error"), free(pattern), NULL);
	entry = readdir(dir);
	result = NULL;
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			;
		else if (match(pattern, entry->d_name))
		{
			result = concatenate_matches_free_s1(result, entry->d_name);
			if (!result)
				return (closedir(dir), free(pattern), NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (result && ft_strlen(result) > 0)
		result[ft_strlen(result) - 1] = '\0';
	return (result);
}

/// @brief loops through the string and expands the wildcard result
void	handle_wildcard_expansion(t_exp *exp, t_data *data)
{
	char	*env_var;
	int		index;

	index = exp->i - 1;
	env_var = list_matching_files(get_pattern((char *)exp->str, exp->i, data));
	if (env_var)
	{
		while (exp->buf_pos > 0 && ((exp->str[exp->buf_pos - 1] != ' ' && \
		!is_operator_symbol(exp->str[exp->buf_pos - 1], ' ')) || in_quote(exp->\
		str, '"', &exp->str[exp->buf_pos - 1]) || in_quote(exp->str, '\'', \
		&exp->str[exp->buf_pos - 1])))
			exp->buf_pos--;
		exp->buffer[exp->buf_pos] = '\0';
		append_to_buffer(exp, env_var);
		exp->buffer[index + ft_strlen(env_var) + 1] = '\0';
		free(env_var);
		while (exp->str[exp->i] && (exp->str[exp->i] != ' ' || \
		in_quote(exp->str, '"', &exp->str[exp->i]) || in_quote(exp->str, '\'', \
		&exp->str[exp->i])) && !is_operator_symbol(exp->str[exp->i], ' '))
			exp->i++;
	}
	else
		append_to_buffer(exp, "*");
}
