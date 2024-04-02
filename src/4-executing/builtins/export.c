/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:38:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 09:57:07 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	export_error_return(int error_code)
{
	if (error_code == 1)
		ft_putstr_fd("export: not enough arguments\n", STDERR_FILENO);
	else if (error_code == 2)
		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
	else if (error_code == 3)
		ft_putstr_fd("export: error setting env variable\n", STDERR_FILENO);
	else if (error_code == 4)
		ft_putstr_fd("export: error allocating memory\n", STDERR_FILENO);
	return (1);
}

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
			return(export_error_return(2));
		var = ft_substr(node[i]->value, 0, equ_pos);
		if (!var)
			return(export_error_return(4));
		value = ft_substr(node[i]->value, equ_pos + 1, \
							ft_strlen(node[i]->value) - equ_pos - 1);
		if (!value)
			return(export_error_return(4));
		if (set_envcp_var(var, value, 1, program_data) != 0)
			return(export_error_return(3));
	}
	return (0);
}
