/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:38:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 13:31:54 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	export_error_return(int error_code, char *arg, t_program_data *data)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	if (error_code == 2)
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	else if (error_code == 3)
		ft_putstr_fd(": error setting env variable\n", STDERR_FILENO);
	else if (error_code == 4)
		ft_putstr_fd(": error allocating memory\n", STDERR_FILENO);
	data->exit_status = 1;
	return (1);
}

bool	is_valid_env_var(char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]) && var[i] != '_')
		return (false);
	while (var[++i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
	}
	return (true);
}

int	execute_export(t_token **tokens, int out_fd, t_program_data *program_data)
{
	int		i;
	char	*equ_pos_char;
	char	*var;
	char	*value;

	i = 0;
	if (!tokens[1])
	{
		while (program_data->envcp[i])
		{
			ft_putstr_fd("declare -x ", out_fd);
			ft_putendl_fd(program_data->envcp[i], out_fd);
			i++;
		}
		return (0);
	}
	while (tokens[++i])
	{
		equ_pos_char = ft_strchr(tokens[i]->value, '=');
		if (!equ_pos_char || equ_pos_char == tokens[i]->value || !is_valid_env_var(tokens[i]->value))
		{
			if (equ_pos_char == tokens[i]->value || !is_valid_env_var(tokens[i]->value))
				export_error_return(2, tokens[i]->value, program_data);
			continue;
		}
		var = ft_substr(tokens[i]->value, 0, equ_pos_char - tokens[i]->value);
		if (!var)
			return (export_error_return(4, NULL, program_data));
		if (ft_isnbr(var))
		{
			export_error_return(2, tokens[i]->value, program_data);
			continue;
		}
		value = ft_substr(tokens[i]->value, equ_pos_char - tokens[i]->value + 1, ft_strlen(tokens[i]->value) - (equ_pos_char - tokens[i]->value) - 1);
		if (!value)
			return (export_error_return(4, NULL, program_data));
		if (set_envcp_var(var, value, 1, program_data) != 0)
			return (export_error_return(3, NULL, program_data));
	}
	return (program_data->exit_status);
}
