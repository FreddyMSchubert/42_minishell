/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 12:12:06 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:33:15 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_valid_variable(const char *var)
{
	int	i;

	if (!ft_isalpha((unsigned char)var[0]) && var[0] != '_')
		return (false);
	i = 0;
	while (var[++i] && var[i] != '$' && (ft_isalnum(var[i]) || var[i] == '_') \
		&& !is_operator_symbol(var[i], ' '))
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
	}
	return (true);
}

/// @brief isolates the variable name from the string
char	*isolate_var(char *var)
{
	int	i;

	i = -1;
	if (!var)
		return (NULL);
	while (var[++i])
	{
		if (var[i] == ' ' || var[i] == '\'' || var[i] == '\"' || var[i] == '$'\
			|| is_operator_symbol(var[i], var[i + 1]) || var[i] == '/' || \
			var[i] == '?')
		{
			var[i] = '\0';
			break ;
		}
	}
	return (var);
}

/// @brief gets the value of an environment variable
char	*get_envcp(const char *env_var, t_data *sh)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = ft_strjoin(env_var, "=");
	if (!tmp)
		exit_error("malloc failed", 1, sh->gc);
	gc_append_element(sh->gc, tmp);
	while (sh->envcp[++i])
	{
		if (ft_strncmp(sh->envcp[i], tmp, ft_strlen(tmp)) == 0)
			return (ft_strdup(sh->envcp[i] + ft_strlen(env_var) + 1));
	}
	return (ft_strdup(""));
}
