/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/20 09:45:08 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
search for $ in string if not single quoted
loop thru env array
if env name matches var name
return env value
*/

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

t_token	**expander(t_token **tokens, char **envp)
{
	int		i;
	int		j;
	int		k;
	char	*env_value;
	char	*var_name;

	i = -1;
	while (tokens[++i])
	{
		if (tokens[i]->type != TOK_S_QUOTE && ft_strchr(tokens[i]->value, '$') != NULL)
		{
			var_name = isolate_var(ft_strdup(ft_strchr(tokens[i]->value, '$') + 1));
			if (!var_name)
			// throw error
			env_value = getenv(var_name);
		}
	}
	return (tokens);
}