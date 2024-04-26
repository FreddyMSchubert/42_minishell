/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:54:08 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 07:19:11 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

bool	is_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	execute_exit(t_tok **tokens, t_data *program_data, int out_fd)
{
	if (tokens[1] && (!is_digit(tokens[1]->val) || \
			ft_strncmp(tokens[1]->val, "", 1) == 0))
	{
		log_err("numeric argument required", "exit", NULL);
		program_data->exit_status = 255;
		program_data->exit_flag = 1;
	}
	else if (tokens[1] && tokens[2])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		program_data->exit_status = 1;
	}
	else if (tokens[1])
	{
		program_data->exit_status = ft_atoi(tokens[1]->val);
		program_data->exit_status %= 256;
		if (program_data->exit_status < 0)
			program_data->exit_status += 256;
	}
	else
		program_data->exit_status = 0;
	program_data->exit_flag = 1;
	if (isatty(fileno(stdin)))
		ft_putstr_fd("exit\n", out_fd);
	return (program_data->exit_status);
}
