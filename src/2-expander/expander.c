/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/20 13:20:52 by nburchha         ###   ########.fr       */
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
	int	i;

	i = -1;
	while (program_data->envcp[++i])
	{
		if (ft_strncmp(program_data->envcp[i], env_var, ft_strlen(env_var)) == 0)
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

char	*get_expanded_str(char *input, char *envcp_value, t_program_data *program_data, char *env_var)
{
	char	*expanded_str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	expanded_str = ft_calloc(ft_strlen(input) + ft_strlen(envcp_value) - ft_strlen(env_var) + 1, sizeof(char));
	if (!expanded_str)
			exit_error("malloc failed", 1, program_data->gc);
	// append_element(program_data->gc, expanded_str);
	while (input[++i])
	{
		if (ft_strncmp(&input[i], "$", 1) == 0)
		{
			ft_strlcpy(&expanded_str[j], envcp_value, ft_strlen(envcp_value) + 1);
			j += ft_strlen(envcp_value);
			i += ft_strlen(env_var);
		}
		else
			expanded_str[j++] = input[i];
	}
	return (expanded_str);
}

t_token	**expander(t_token **tokens, t_program_data *program_data)
{
	int		i;
	char	*envcp_value;
	char	*env_var;
	// char	*tmp;

	i = -1;
	while (tokens[++i])
	{
		if (tokens[i]->type != TOK_S_QUOTE && ft_strchr(tokens[i]->value, '$') != NULL)
		{
			env_var = isolate_var(ft_strdup(ft_strchr(tokens[i]->value, '$') + 1));
			if (!env_var)
				exit_error("malloc failed", 1, program_data->gc);
			printf("env_var: %s\n", env_var);
			// append_element(program_data->gc, env_var);
			envcp_value = get_envcp(env_var, program_data);
			printf("envcp_value: %s\n", envcp_value);
			if (!envcp_value)
				exit_error("malloc failed", 1, program_data->gc);
			// append_element(program_data->gc, envcp_value);
			tokens[i]->value = get_expanded_str(tokens[i]->value, envcp_value, program_data, env_var);
		}
	}
	
	return (tokens);
}
