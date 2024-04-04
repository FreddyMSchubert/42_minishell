/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:44:01 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/04 17:06:12 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*concatenate_matches_free_s1(char *s1, const char *s2)
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
	new_str = malloc(s1_len + s2_len + 2); // +2 for space and '\0'
	if (!new_str)
		return (NULL);
	if (s1)
	{
		ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
		free(s1);
	}
	ft_strlcpy(new_str + s1_len, s2, ft_strlen(s2) + 1);
	new_str[s1_len + s2_len] = ' '; // Add space
	new_str[s1_len + s2_len + 1] = '\0';
	return (new_str);
}

int	match(const char *pattern, const char *filename)
{
	const char	*p = pattern;
	const char	*f = filename;
	const char	*star = NULL;
	const char	*sf = NULL;

	while (*f)
	{
		if (*p == *f && p++ && f++)
			;
		else if (*p == '*')
		{
			star = p++;
			sf = f;
		}
		else if (star)
		{
			p = star + 1;
			f = ++sf;
		}
		else
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0'); // Successful match if end of pattern
}

char	*list_matching_files(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*result;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir() error"), NULL);
	entry = readdir(dir);
	result = NULL;
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.')
			;
		else if (match(pattern, entry->d_name))
		{
			result = concatenate_matches_free_s1(result, entry->d_name);
			if (!result)
				return (closedir(dir), NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (result && ft_strlen(result) > 0)
		result[ft_strlen(result) - 1] = '\0';
	printf("result: %s\n", result);
	return (result);
}
