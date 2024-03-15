/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/15 13:37:44 by nburchha         ###   ########.fr       */
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

static char	*get_expanded_str(char *input, char *envcp_value,
		t_program_data *program_data, char *env_var)
{
	char	*expanded_str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	expanded_str = ft_calloc(ft_strlen(input) + ft_strlen(envcp_value)
			- ft_strlen(env_var) + 1, sizeof(char));
	if (!expanded_str)
		exit_error("malloc failed", 1, program_data->gc);
	gc_append_element(program_data->gc, expanded_str);
	while (input[++i])
	{
		if (ft_strncmp(&input[i], "$", 1) == 0)
		{
			ft_strlcpy(&expanded_str[j], envcp_value, ft_strlen(envcp_value)
				+ 1);
			j += ft_strlen(envcp_value);
			i += ft_strlen(env_var);
		}
		else
			expanded_str[j++] = input[i];
	}
	return (expanded_str);
}

static bool	is_in_quote(char *str, char *quote, char *current_char)
{
	int		i;
	bool	in_quote;

	i = -1;
	in_quote = false;
	while (&str[i] != current_char && str[++i])
	{
		if (ft_strchr(quote, str[i]) != NULL)
			in_quote = !in_quote;
	}
	return (in_quote);
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
		if (!d_quote && str[i] != '\'' && !s_quote && str[i] != '\"')
			new_str[j++] = str[i];
	}
	return (new_str);
}

// wildcard, need to take* with the rest in front or behind delimitted by spaces

t_list	*expander(t_list *tokens, t_program_data *program_data)
{
	char	*envcp_value;
	char	*env_var;
	t_list	*tok;
	t_token	*token;
	char	*tmp;

	tok = tokens;
	while (tok != NULL)
	{
		token = (t_token *)tok->content;
		//exit code
		if (ft_strnstr(token->value, "$?",
				ft_strlen(token->value)) != NULL
			&& !is_in_quote(token->value, "\'", ft_strnstr(token->value,
					"$?", ft_strlen(token->value))))
		{
			envcp_value = ft_itoa(program_data->exit_status);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, envcp_value);
			token->value = get_expanded_str(token->value, envcp_value,
					program_data, "?");
		}
		//expansion
		else if (ft_strchr(token->value, '$') != NULL
			&& !is_in_quote(token->value, "\'", ft_strchr(token->value,
					'$')))
		{
			env_var = isolate_var(ft_strdup(ft_strchr(token->value, '$')
						+ 1));
			if (!env_var)
				exit_error("malloc failed", 1, program_data->gc);
			// printf("env_var: %s\n", env_var);
			gc_append_element(program_data->gc, env_var);
			envcp_value = get_envcp(env_var, program_data);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, envcp_value);
			tmp = get_expanded_str(token->value, envcp_value,
					program_data, env_var);
			if (tmp)
				token->value = tmp;
		}
		//wildcard
		else if (token->type == TOK_WORD && ft_strchr(token->value,
				'*') != NULL && !is_in_quote(token->value, "'",
				ft_strchr(token->value, '*'))
			&& !is_in_quote(token->value, "\"", ft_strchr(token->value,
				'*'))) // wildcard, need to take* with the rest in front or behind delimitted by spaces
		{
			tmp = list_matching_files(token->value);
			if (tmp)
			{
				token->value = tmp;
				gc_append_element(program_data->gc, token->value);
			}
		}
		tok = tok->next;
	}
	return (tokens);
}
