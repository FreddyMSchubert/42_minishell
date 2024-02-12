/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:40:24 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/12 13:27:39 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

// static int is_operator_symbol(char c)
// {
// 	// if (c == '\'' || c == '\"' || c == '<' || \
// 	// 	c == '>' || c == '|' || c == '&' || c == ';')
// 	// 	return (0);
// 	return (1);
// }

static int	is_operator_symbol(char c)
{
    // Define your operator symbols here
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

/// @brief counts how many pointers are needed in pointer array
static int	count_words(const char *s)
{
	int	i;
	int	count;
	int	in_word;
	int	in_quote;

	i = -1;
	count = 0;
	in_word = 0;
	in_quote = 0;
	while (s[++i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			if (!in_quote)
			{
				in_quote = 1;
				count++;
			}
			else if (s[i - 1] != '\\')
				in_quote = 0;
		}
		else if (is_operator_symbol(s[i]))
		{
			count++;
			in_word = 0;
		}
		else if (!ft_isspace(s[i]))
		{
			if (!in_word && !in_quote)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
	}
	return (count);
}

/// @brief gets start point of string to be split,
/// determines end point by the next delim in s
/// @return splitted string
// static char	*make_split_str(const char *s, char delim, int *i)
// {
// 	size_t	size;

// 	size = *i;
// 	while (s[*i] != delim && s[*i])
// 		*i += 1;
// 	*i -= 1;
// 	return (ft_substr(s, size, *i - size + 1));
// }

/// @brief frees the 2d array in case of a allocation fail
// void	free_split(char **split)
// {
// 	int	i;

// 	i = 0;
// 	if (split == NULL)
// 		return ;
// 	while (split[i])
// 	{
// 		free(split[i]);
// 		split[i] = NULL;
// 		i++;
// 	}
// 	free(split);
// 	split = NULL;
// }

/// @brief splits a string into an array of strings, using spaces,
/// single quotes and double quotes as delimiters. inside single
/// quotes: everything is a string,
/// inside double quotes: everything but $ is a string
// char **ms_split(char *input)
// {
// 	int				word_count;
// 	char			**result;
// 	int				i;
// 	int				j;
// 	int				flag;

// 	i = -1;
// 	j = 0;
// 	flag = 0;
// 	word_count = count_words(input);
// 	result = ft_calloc(word_count + 1, sizeof(char *));
// 	if (!result)
// 		return (NULL);
// 	result[word_count] = NULL;
// 	while (result[++i] && j < word_count)
// 	{
// 		if (result[i] != c)
// 		{
// 			result[j++] = make_split_str(input, &i);
// 			if (!result[j - 1])
// 				return (free_split(result), NULL);
// 		}
// 	}
// 	return (result);
// }


// int main(void)
// {
// 	char *input = "echo \"hel$$USERlo world\"";
// 	printf("%d\n", count_words(input));
// 	return 0;
// }