/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:48:54 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/21 18:51:54 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static bool	is_valid_variable(char *var)
{
	int	i;

	if (!ft_isalpha((unsigned char)var[0]) && var[0] != '_')
		return (false);
	i = 0;
	while (var[++i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
	}
	return (true);
}

int	execute_unset(t_token **node, t_program_data *program_data)
{
	int	i;

	i = 1;
	while (node[i] != NULL)
	{
		// printf("node[i]->value: %s\n", node[i]->value);
		if (ft_strchr(node[i]->value, '=') != NULL || 
			!is_valid_variable(node[i]->value) ||
			ft_strncmp(node[i]->value, "=", 1) == 0 || 
			ft_strncmp(node[i]->value, "?", 1) == 0 || 
			ft_strncmp(node[i]->value, "$", 1) == 0 || 
			ft_strncmp(node[i]->value, "", 1) == 0)
		{
			ft_putstr_fd("unset: not a valid identifier\n", STDERR_FILENO);
			program_data->exit_status = 1;
			return (1);
		}
		else if (ft_strncmp(node[i]->value, "_", 2) == 0)
		{
			ft_putstr_fd("unset: cannot unset _\n", STDERR_FILENO);
			program_data->exit_status = 1;
			return (1);
		}
		delete_envcp_var(node[i]->value, program_data->envcp);
		i++;
	}
	program_data->exit_status = 0;
	return (0);
}
