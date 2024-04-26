/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:42:01 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 18:07:39 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	gets a start point of a string to be split,
			determines the end point by the next delimiter in s
	@return	the splitted string
*/
static char	*make_split_str(const char *s, int *i, t_data *data)
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

/*
	@brief	frees the 2d array in case of a allocation fail
*/
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

/*
	@brief	splits a string into an array of strings, using spaces,
			single quotes and double quotes as delimiters.
			inside single quotes: everything is a string
	@param	input: the string to be split
	@param	data: the struct containing the garbage collector
	@return	the array of strings
*/
char	**ms_split(char *input, t_data *data)
{
	int				word_count;
	char			**result;
	int				i;
	int				j;

	i = -1;
	j = 0;
	word_count = count_tokens(input);
	if (word_count == -1)
		return (NULL);
	result = ft_calloc((word_count + 1), sizeof(char *));
	if (!result)
		return (NULL);
	gc_append_element(data->gc, result);
	while (input[++i] && j < word_count)
	{
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
