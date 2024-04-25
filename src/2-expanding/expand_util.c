/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:07:07 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/25 20:27:53 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
			new_str[j++] = '"';
		}
		else
			new_str[j++] = envcp_value[i];
	}
	// printf("new_str: %s\n", new_str);
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