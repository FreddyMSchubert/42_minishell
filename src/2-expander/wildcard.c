/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:44:01 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/24 13:00:35 by niklasburch      ###   ########.fr       */
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
	new_str = malloc(s1_len + s2_len + 4); // +4 for two single quotes, space and '\0'
	if (!new_str)
		return (NULL);
	if (s1)
	{
		ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
		free(s1);
	}
	new_str[s1_len] = '\''; // Add opening single quote
	ft_strlcpy(new_str + s1_len + 1, s2, ft_strlen(s2) + 1);
	new_str[s1_len + s2_len + 1] = '\''; // Add closing single quote
	new_str[s1_len + s2_len + 2] = ' '; // Add space
	new_str[s1_len + s2_len + 3] = '\0';
	return (new_str);
}

// int	match(const char *pattern, const char *filename)
// {
// 	const char	*p = pattern;
// 	const char	*f = filename;
// 	const char	*star = NULL;
// 	const char	*sf = NULL;

// 	printf("pattern: %s, filename: %s\n", pattern, filename);
// 	while (*f)
// 	{
// 		printf("p: %c, f: %c\n", *p, *f);
// 		if (*p == *f && p++ && f++)
// 			;
// 		else if (*p == '\\' && *(p + 1) == '*' && *f == '*') // Handle literal * in filename
// 		{
// 			printf("escaped: p: %c, f: %c\n", *p, *f);
// 			p += 2;
// 			f++;
// 		}
// 		else if (*p == '*' && *(p - 1) != '\\')
// 		{
// 			star = p++;
// 			sf = f;
// 		}
// 		else if (star)
// 		{
// 			p = star + 1;
// 			f = ++sf;
// 		}
// 		else
// 			return (0);
// 	}
// 	while (*p == '*' && *(p - 1) != '\\')
// 		p++;
// 	return (*p == '\0'); // Successful match if end of pattern
// }

int	match(const char *pattern, const char *filename)
{
	const char	*p = pattern;
	const char	*f = filename;
	const char	*star = NULL;
	const char	*sf = NULL;

	// printf("pattern: %s, filename: %s\n", pattern, filename);
	while (*f)
	{
		// printf("p: %c, f: %c\n", *p, *f);
		if (star && !(*star + 1)) // If pattern ends with *, match rest of filename
				return (1);
		if (*p == *f && *p != '*' && p++ && f++)
			;
		else if (*p == '\\' && *(p + 1) == '*' && *f == '*') // Handle literal * in filename
		{
			// printf("escaped: p: %c, f: %c\n", *p, *f);
			p += 2;
			f++;
		}
		else if (*p == '*' && (p == pattern || *(p - 1) != '\\'))
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
	while (*p == '*' && (p == pattern || *(p - 1) != '\\'))
		p++;
	return (*p == '\0'); // Successful match if end of pattern
}

char	*list_matching_files(char *pattern)
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
			// printf("match: %s\n", entry->d_name);
			result = concatenate_matches_free_s1(result, entry->d_name);
			if (!result)
				return (closedir(dir), free(pattern), NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (result && ft_strlen(result) > 0)
		result[ft_strlen(result) - 1] = '\0';
	free(pattern);
	return (result);
}
