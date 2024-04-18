/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:40:52 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/18 19:59:03 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_env(t_program_data *program_data, int out_fd)
{
	int	i;

	i = 0;
	while (program_data->envcp[i])
	{
		ft_putstr_fd(program_data->envcp[i], out_fd);
		ft_putchar_fd('\n', out_fd);
		i++;
	}
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0);
}
