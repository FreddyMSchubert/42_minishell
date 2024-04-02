/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/02 14:01:10 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
search for $ in string if not single quoted
loop thru envcp array
if envcp name matches var name
return envcp value
*/

char	*get_envcp(char *env_var, t_program_data *program_data)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = ft_strjoin(env_var, "=");
	if (!tmp)
		exit_error("malloc failed", 1, program_data->gc);
	gc_append_element(program_data->gc, tmp);
	while (program_data->envcp[++i])
	{
		if (ft_strncmp(program_data->envcp[i], tmp, ft_strlen(tmp)) == 0)
			return (ft_strdup(program_data->envcp[i] + ft_strlen(env_var) + 1));
	}
	return (ft_strdup(""));
}

char	*isolate_var(char *var)
{
	int	i;

	i = -1;
	if (!var)
		return (NULL);
	while (var[++i])
	{
		if (var[i] == ' ' || var[i] == '\'' || var[i] == '\"' || var[i] == '$'
			|| is_operator_symbol(var[i], var[i + 1]))
		{
			var[i] = '\0';
			break ;
		}
	}
	return (var);
}

// static char	*get_expanded_str(char *input, char *envcp_value,
// 		t_program_data *program_data, char *env_var)
// {
// 	char	*expanded_str;
// 	int		i;
// 	int		j;

// 	i = -1;
// 	j = 0;
// 	expanded_str = ft_calloc(ft_strlen(input) + ft_strlen(envcp_value)
// 			- ft_strlen(env_var) + 1, sizeof(char));
// 	if (!expanded_str)
// 		exit_error("malloc failed", 1, program_data->gc);
// 	gc_append_element(program_data->gc, expanded_str);
// 	while (input[++i])
// 	{
// 		if (ft_strncmp(&input[i], "$", 1) == 0)
// 		{
// 			ft_strlcpy(&expanded_str[j], envcp_value, ft_strlen(envcp_value)
// 				+ 1);
// 			j += ft_strlen(envcp_value);
// 			i += ft_strlen(env_var);
// 		}
// 		else
// 			expanded_str[j++] = input[i];
// 	}
// 	return (expanded_str);
// }

static bool	is_in_quote(char *str, char *quote, char *current_char)
{
	int		i;
	bool	d_quote;
	bool	s_quote;

	i = -1;
	d_quote = false;
	s_quote = false;
	while (&str[i] != current_char && str[++i])
	{
		if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
	}
	if (quote[0] == '\"')
		return (d_quote);
	else
		return (s_quote);
}

char	*get_rid_of_quotes(char *str, t_program_data *program_data)
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
	// printf("get_rid_of_quotes: %s, len: %zu\n", str, ft_strlen(str));
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	gc_append_element(program_data->gc, new_str);
	while (str[++i])
	{
		if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else
			new_str[j++] = str[i];
		// printf("str[%d]: %c\n", i, str[i]);
	}
	new_str[j] = '\0';
	printf("get_rid_of_quotes: %s\n", new_str);
	return (new_str);
}

/// @brief retrieves the pattern of a wildcard
char	*get_pattern(char *str, int index)
{
	int	i;

	i = index;
	while (str[i] && str[i] != ' ' && !is_operator_symbol(str[i], ' '))
		i++;
	while (index > 0 && str[index] != ' ' && !is_operator_symbol(str[index], ' '))
		index--;
	index++;
	return (ft_substr(&str[index], 0, i - index));
}


//does the same as the above function, but with a string instead of a list, expands all $ and wildcards
char *expand_values(char *str, t_program_data *program_data)
{
	int	i;
	char	*envcp_value;
	char	*env_var;
	char	*new_str;
	char	*tmp;

	i = -1;
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	while (str[++i])
	{
		if (str[i] == '~')
			new_str = ft_strjoinfree(new_str, get_envcp("HOME", program_data));
		else if (ft_strnstr(&str[i], "$?", 2) != NULL
			&& !is_in_quote(str, "\'", &str[i]))
		{
			envcp_value = ft_itoa(program_data->exit_status);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			new_str = ft_strjoinfree(new_str, envcp_value);
			i++;
		}
		else if (ft_strchr(&str[i], '$') == &str[i]
			&& !is_in_quote(str, "\'", &str[i]))
		{
			env_var = isolate_var(ft_strdup(ft_strchr(&str[i], '$') + 1));
			if (!env_var)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, env_var);
			i += ft_strlen(env_var);
			envcp_value = get_envcp(env_var, program_data);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			new_str = ft_strjoinfree(new_str, envcp_value);
		}
		//wildcard
		else if (str[i] == '*' && !is_in_quote(str, "'", &str[i])
			&& !is_in_quote(str, "\"", &str[i])) // wildcard, need to take* with the rest in front or behind delimitted by spaces
		{
			printf("str[i]: .%c.\n", str[i]);
			tmp = list_matching_files(get_pattern(str, i));
			printf("tmp: %s\n", tmp);
			if (tmp)
			{
				new_str = ft_strjoinfree(new_str, tmp);
				while (str[i] && str[i] != ' ' && !is_operator_symbol(str[i], ' '))
					i++;
				if (str[i] && (str[i] == ' ' || is_operator_symbol(str[i], ' ')))
					new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 0, 1));
			}
			else
				new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 0, 1));
		}
		else
			new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 0, 1));
	}
	gc_append_element(program_data->gc, new_str);
	printf("new_str: %s\n", new_str);
	return (new_str);
}
