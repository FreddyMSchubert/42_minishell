/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/14 12:05:54 by fschuber         ###   ########.fr       */
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
		if (var[i] == ' ')
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

// wildcard, need to take* with the rest in front or behind delimitted by spaces

t_list	*expander(t_list *tokens, t_program_data *program_data)
{
	char	*envcp_value;
	char	*env_var;
	t_list	*tok;
	t_token	*token;

	tok = tokens;
	while (tok != NULL)
	{
		token = tok->content;
		if (ft_strnstr(token->value, "$?",
				ft_strlen(token->value)) != NULL)
		{
			envcp_value = ft_itoa(program_data->exit_status);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, envcp_value);
			token->value = get_expanded_str(token->value, envcp_value,
					program_data, "?");
		}
		else if (ft_strchr(token->value, '$') != NULL)
		{
			env_var = isolate_var(ft_strdup(ft_strchr(token->value, '$')
						+ 1));
			if (!env_var)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, env_var);
			envcp_value = get_envcp(env_var, program_data);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			gc_append_element(program_data->gc, envcp_value);
			token->value = get_expanded_str(token->value, envcp_value,
					program_data, env_var);
		}
		else if (token->type == TOK_WORD && ft_strchr(token->value,
				'*') != NULL)
		{
			token->value = list_matching_files(token->value);
			if (!token->value)
			{
				token->value = ft_strdup("*");
				gc_append_element(program_data->gc, token->value);
			}
		}
		tok = tok->next;
	}
	return (tokens);
}
