/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:48:54 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/28 14:44:26 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_unset(t_token **node, t_program_data *program_data, int cmd_start_index)
{
	int	i;

	i = cmd_start_index + 1;
	while (node[i] != NULL)
	{
		if (delete_envcp_var(node[i]->value, program_data->envcp) != 0)
			return (1); // handle error
		i++;
	}
	return (0);
}
