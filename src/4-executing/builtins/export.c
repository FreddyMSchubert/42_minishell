/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:38:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/26 09:27:09 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_export(t_token **node, t_program_data *program_data, int cmd_start_index)
{
	int		i;
	int		equ_pos;
	char	*equ_pos_char;
	char	*var;
	char	*value;

	i = cmd_start_index;
	while (node[++i])
	{
		equ_pos_char = ft_strchr(node[i]->value, '=');
		if (equ_pos_char != NULL)
			equ_pos = equ_pos_char - node[i]->value;
		else
			return (-1); // handle error
		var = ft_substr(node[i]->value, 0, equ_pos);
		if (!var)
			return (-2); // handle error
		value = ft_substr(node[i]->value, equ_pos + 1, \
							ft_strlen(node[i]->value) - equ_pos - 1);
		if (!value)
			return (-3); // handle error
		if (set_envcp_var(var, value, 1, program_data) != 0)
			return (-42); // handle error
	}
	return (0);
}
