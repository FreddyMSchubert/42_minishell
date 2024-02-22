/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:38:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:02:02 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_export(t_token **node, t_program_data *program_data)
{
	int		i;
	int		equ_pos;
	char	*var;
	char	*value;

	i = 0;
	while (node[++i])
	{
		equ_pos = ft_strchr(node[i]->value, '=') - node[i]->value;
		if (equ_pos == 0)
			return (-1);
		var = ft_substr(node[i]->value, 0, equ_pos);
		if (!var)
			return (-2);
		value = ft_substr(node[i]->value, equ_pos + 1, \
							ft_strlen(node[i]->value) - equ_pos - 1);
		if (set_envcp_var(var, value, 1, program_data) != 0)
			return (-42); // handle error
	}
	return (0);
}
