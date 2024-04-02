/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:48:54 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 13:19:58 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_unset(t_token **node, t_program_data *program_data)
{
	int	i;

	i = 1;
	while (node[i] != NULL)
	{
		if (delete_envcp_var(node[i]->value, program_data->envcp) != 0)
			return (builtin_err("unset", -5, "fail unsetting var"), 1);
		i++;
	}
	return (0);
}
