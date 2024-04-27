/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:48:54 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/27 12:47:33 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static bool	is_valid_unset_variable(char *var)
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

int	execute_unset(t_tok **node, t_data *data)
{
	int	i;

	i = 1;
	while (node[i] != NULL)
	{
		if (ft_strchr(node[i]->val, '=') != NULL || \
			!is_valid_unset_variable(node[i]->val) || \
			ft_strncmp(node[i]->val, "=", 1) == 0 || \
			ft_strncmp(node[i]->val, "?", 1) == 0 || \
			ft_strncmp(node[i]->val, "$", 1) == 0 || \
			ft_strncmp(node[i]->val, "", 1) == 0)
		{
			log_err("not a valid identifier", "unset", node[i]->val);
			data->exit_status = 1;
			return (1);
		}
		delete_envcp_var(node[i]->val, data->envcp);
		i++;
	}
	data->exit_status = 0;
	return (0);
}
