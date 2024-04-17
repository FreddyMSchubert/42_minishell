/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:42:01 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/17 09:41:15 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @brief counts how many pointers are needed in pointer array
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
		// printf("%zu\n", ft_strlen(s));
		if ((s[i] == '"' && in_quote != 2) || (s[i] == '\'' && in_quote != 1))
		{
			// printf("quote: %c\n", s[i]);
			if (in_quote == 0)
			{
				if (s[i] != '\"')
					in_quote = 2;
				else
					in_quote = 1;
				// count++;
				// in_word = 0;
			}
			else
				in_quote = 0;
		}
		if (is_operator_symbol(s[i], s[i + 1]))
		{
			// printf("operator: %c\n", s[i]);
			if (is_operator_symbol(s[i], s[i + 1]) == 2)
				i++;
			count++;
			in_word = 0;
		}
		else if (!ft_isspace(s[i]) && !in_word && !in_quote) //count new word
		{
			count++;
			in_word = 1;
			// printf("wortanfang: %c\n", s[i]);
		}
		else if (ft_isspace(s[i]) && !in_quote)
			in_word = 0;
	}
	// if (in_quote)
	// 	return (-1);
	return (count);
}

/// @brief gets start point of string to be split,
/// determines end point by the next delim in s
/// @return splitted string
static char	*make_split_str(const char *s, int *i, t_program_data *data)
{
	size_t	size;
	int		quote;
	char	*ret;

	size = *i;
	quote = 0;
	while (s[*i])
	{
		if (s[*i] == '\'' || s[*i] == '"')
		{
			if (quote == 0)
				quote = s[*i];
			else if (quote == s[*i])
				quote = 0;
		}
		if (ft_isspace(s[*i]) && !quote)
			break ;
		*i += 1;
	}
	*i -= 1;
	ret = ft_substr(s, size, *i - size + 1);
	if (!ret)
		exit_error("Memory allocation failed", 1, data->gc);
	gc_append_element(data->gc, ret);
	return (ret);
}

/// @brief frees the 2d array in case of a allocation fail
void	free_split(char **split)
{
	int	i;

	i = 0;
	if (split == NULL)
		return ;
	while (split[i])
	{
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	free(split);
	split = NULL;
}

/// @brief splits a string into an array of strings, using spaces,
/// single quotes and double quotes as delimiters. inside single
/// quotes: everything is a string
char	**ms_split(char *input, t_program_data *data)
{
	int				word_count;
	char			**result;
	int				i;
	int				j;

	i = -1;
	j = 0;
	word_count = count_tokens(input);
	// printf("word_count: %d\n", word_count);
	if (word_count == -1)
		return (NULL);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	gc_append_element(data->gc, result);
	// result[word_count] = NULL;
	while (input[++i] && j < word_count)
	{
		// printf("input[i]: %s\n", &input[i]);
		if (is_operator_symbol(input[i], input[i + 1]) || !ft_isspace(input[i]))
		{
			result[j++] = make_split_str(input, &i, data);
			if (!result[j - 1])
				return (free_split(result), NULL);
		}
	}
	result[j] = NULL;
	return (result);
}
