/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:48:54 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/18 22:27:22 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_unset(t_token **node, t_program_data *program_data)
{
	int	i;

	i = 1;
	while (node[i] != NULL)
	{
		delete_envcp_var(node[i]->value, program_data->envcp);
		i++;
	}
	program_data->exit_status = 0;
	return (0);
}
