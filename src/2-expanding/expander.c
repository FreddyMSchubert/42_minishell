/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/21 18:52:14 by nburchha         ###   ########.fr       */
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
			|| is_operator_symbol(var[i], var[i + 1]) || var[i] == '/' || var[i] == '?')
		{
			var[i] = '\0';
			break ;
		}
	}
	return (var);
}

bool	is_in_quote(char *str, char quote, char *current_char)
{
	int		i;
	bool	d_quote;
	bool	s_quote;

	i = -1;
	d_quote = false;
	s_quote = false;
	if (!quote || !str || !current_char)
		return (false);
	while (&str[i] != current_char && str[++i])
	{
		if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
	}
	if (quote == '\"')
		return (d_quote);
	else
		return (s_quote);
}

char	*get_rid_of_quotes(char *str)
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
		if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '\"' && !s_quote)
			d_quote = !d_quote;
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	return (new_str);
}

/// @brief retrieves the pattern of a wildcard
char	*get_pattern(char *str, int index, t_program_data *program_data)
{
	int	i;
	char	*ret;

	i = index;
	while (str[i] && (str[i] != ' ' || is_in_quote(str, '"', &str[i]) || is_in_quote(str, '\'', &str[i])) && !is_operator_symbol(str[i], ' '))
		i++;
	while (index > 0 && (str[index] != ' ' || is_in_quote(str, '"', &str[index]) || is_in_quote(str, '\'', &str[index])) && !is_operator_symbol(str[index], ' '))
		index--;
	ret = ft_substr(&str[index + 1], 0, i - index + 1);
	gc_append_element(program_data->gc, ret);
	// ret = expand_values(ret, program_data, true);
	// printf("pattern: .%s.\n", ret);
	return (get_rid_of_quotes(ret));
}

int	find_closing_quote(char *str, int *i)
{
	int		j;
	char	quote;

	j = 1;
	quote = str[0];
	while (str[j] && str[j] != quote)
		j++;
	*i += j;
	return (j);
}

char	*quote_operators(char *envcp_value)
{
	int		i;
	int		j;
	char	*new_str;
	int		is_op;

	i = -1;
	j = 0;
	new_str = ft_calloc(ft_strlen(envcp_value) * 2 + 1, sizeof(char));
	if (!new_str)
		return (free(envcp_value), NULL);
	while (envcp_value[++i])
	{
		is_op = is_operator_symbol(envcp_value[i], envcp_value[i + 1]);
		if (is_op)
		{
			new_str[j++] = '"';
			new_str[j++] = envcp_value[i];
			if (is_op == 2)
				new_str[j++] = envcp_value[++i];
			// i += is_op + 1;
			new_str[j++] = '"';
		}
		else
			new_str[j++] = envcp_value[i];
	}
	free (envcp_value);
	return (new_str);
}

bool	is_valid_variable(char *var)
{
	int	i;

	if (!ft_isalpha((unsigned char)var[0]) && var[0] != '_')
		return (false);
	i = 0;
	while (var[++i] && var[i] != '$' && (ft_isalnum(var[i]) || var[i] == '_') && !is_operator_symbol(var[i], ' ')) // && var[i] != '\'' && var[i] != '\"'
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
	}
	return (true);
}

//checks if a $ or wildcard should be expanded and if the token before is a redirection operator
bool should_expand(char *str, int i, char expansion_type)
{
	int		j;

	//search for redirection token before, to return false if there is one
	j = i;
	while (j > 0 && !is_operator_symbol(str[j], ' ') && !ft_isspace(str[j]))
		j--;
	while (j > 0 && ft_isspace(str[j]))
		j--;
	if (j > 0 && ft_strnstr(&str[j - 1], "<<", 2) && !is_in_quote(str, '\"', &str[j - 1]) && !is_in_quote(str, '\'', &str[j - 1]))
		return (false);
	if (expansion_type == '~' && (ft_isspace(str[i + 1]) || !str[i + 1] || str[i + 1] == '/') && (i == 0 || ft_isspace(str[i - 1]) || is_operator_symbol(str[i - 1], ' ')) && !is_in_quote(str, '\'', &str[i]) && !is_in_quote(str, '\"', &str[i]) && !is_in_quote(str, '`', &str[i]) && !is_in_quote(str, '$', &str[i]) && !is_in_quote(str, '\'', &str[i]) && !is_in_quote(str, '"', &str[i]))
		return (true);
	else if (expansion_type == '?' && ft_strnstr(&str[i], "$?", 2) != NULL && \
			!is_in_quote(str, '\'', &str[i]))
		return (true);
	else if (expansion_type == '"' && str[i] == '$' && \
			!is_in_quote(str, '\"', &str[i]) && !is_in_quote(str, '\'', \
			&str[i]) && (str[i + 1] == '\"' || str[i + 1] == '\''))
		return (true);
	else if (expansion_type == '$' && is_valid_variable(&str[i + 1]) &&\
			!is_in_quote(str, '\'', &str[i]))
		return (true);
	// else if ((expansion_type == 's' && str[i] == '$' && ft_isspace(str[i + 1])) \
	// 		|| (str[i] == '$' && !str[i + 1]))
	else if (expansion_type == 's' && !is_valid_variable(&str[i + 1]) && \
			!is_in_quote(str, '\'', &str[i]) && !is_in_quote(str, '\"', &str[i]))
		return (true);
	else if (expansion_type == '*' && !is_in_quote(str, '\'', \
			&str[i]) && !is_in_quote(str, '\"', &str[i]))
		return (true);
	return (false);
}

//expands all $ and wildcards in a string
char *expand_values(char *str, t_program_data *program_data, bool heredoc)
{
	int	i;
	char	*envcp_value;
	char	*env_var;
	char	*new_str;

	i = -1;
	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	while (str[++i])
	{
		if (str[i] == '~' && should_expand(str, i, '~'))
			new_str = ft_strjoinfree(new_str, get_envcp("HOME", program_data));
		else if (str[i] == '$' && should_expand(str, i, '?'))
		{
			envcp_value = ft_itoa(program_data->exit_status);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			new_str = ft_strjoinfree(new_str, envcp_value);
			i++;
		}
		// else if (ft_strnstr(&str[i], "\"$\"", 3))
		// {
		// 	new_str = ft_strjoinfree(new_str, ft_strdup("\"$\""));
		// 	i += 2;
		// }
		//when $ is found and afterwards theres a quote, dont search for env var, but just replace the $ with nothing
		else if (str[i] == '$' && should_expand(str, i, '"'))
			new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 1, find_closing_quote(&str[i + 1], &i)));
		// else if(str[i] == '$' && should_expand(str, i, 's'))
		// {
		// 	new_str = ft_strjoinfree(new_str, &str[i]);
		// }
		else if (str[i] == '$' && should_expand(str, i, '$'))
		{
			env_var = isolate_var(ft_strdup(ft_strchr(&str[i], '$') + 1));
			if (!env_var)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, env_var);
			i += ft_strlen(env_var);
			envcp_value = get_envcp(env_var, program_data);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			envcp_value = quote_operators(envcp_value);
			new_str = ft_strjoinfree(new_str, envcp_value);
		}
		//wildcard
		else if (str[i] == '*' && should_expand(str, i, '*') && !heredoc) // wildcard, need to take* with the rest in front or behind delimitted by spaces
		{
			// printf("str[i]: .%c.\n", str[i]);
			env_var = list_matching_files(get_pattern(str, i, program_data));
			// printf("wildcard: %s\n", env_var);
			if (env_var)
			{
				// printf("env_var: %s\n", env_var);
				int index = ft_strlen(new_str) - 1;
				while (index > 0 && (str[index] != ' ' || is_in_quote(str, '"', &str[index]) || is_in_quote(str, '\'', &str[index])) && !is_operator_symbol(str[index], ' '))
					index--;
				new_str[index + 1] = '\0';
				new_str = ft_strjoinfree(new_str, ft_strdup("\""));
				new_str = ft_strjoinfree(new_str, env_var);
				new_str = ft_strjoinfree(new_str, ft_strdup("\""));
				while (str[i] && (str[i] != ' ' || is_in_quote(str, '"', &str[i]) || is_in_quote(str, '\'', &str[i])) && !is_operator_symbol(str[i], ' '))
					i++;
				if (str[i] && (str[i] == ' ' || is_operator_symbol(str[i], ' ')))
					new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 0, 1));
				// printf("new_str: %s\n", new_str);
			}
			else
				new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 0, 1));
		}
		else
			new_str = ft_strjoinfree(new_str, ft_substr(&str[i], 0, 1));
	}
	gc_append_element(program_data->gc, new_str);
	// printf("new_str: %s\n", new_str);
	return (new_str);
}
