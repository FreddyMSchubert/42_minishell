/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:38:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/17 13:14:09 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	export_error_return(int error_code, char *arg)
{
	ft_putstr_fd("export: ", STDERR_FILENO);
	if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	if (error_code == 1)
		ft_putstr_fd(": not enough arguments\n", STDERR_FILENO);
	else if (error_code == 2)
		ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	else if (error_code == 3)
		ft_putstr_fd(": error setting env variable\n", STDERR_FILENO);
	else if (error_code == 4)
		ft_putstr_fd(": error allocating memory\n", STDERR_FILENO);
	return (1);
}

int	execute_export(t_token **tokens, t_program_data *program_data)
{
	int		i;
	int		equ_pos;
	char	*equ_pos_char;
	char	*var;
	char	*value;

	i = 0;
	while (tokens[++i])
	{
		equ_pos_char = ft_strchr(tokens[i]->value, '=');
		if (equ_pos_char != NULL && equ_pos_char > tokens[i]->value)
			equ_pos = equ_pos_char - tokens[i]->value;
		else
		{
			if (equ_pos_char == NULL)
				continue;
			else
				return (export_error_return(2, tokens[i]->value));
		}
		var = ft_substr(tokens[i]->value, 0, equ_pos);
		if (!var)
			return (export_error_return(4, NULL));
		value = ft_substr(tokens[i]->value, equ_pos + 1, \
							ft_strlen(tokens[i]->value) - equ_pos - 1);
		if (!value)
			return (export_error_return(4, NULL));
		if (set_envcp_var(var, value, 1, program_data) != 0)
			return (export_error_return(3, NULL));
	}
	return (0);
}
