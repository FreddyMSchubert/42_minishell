/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:40:52 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 07:51:18 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_env(t_program_data *program_data)
{
	int	i;

	i = 0;
	while (program_data->envcp[i])
	{
		ft_printf("%s\n", program_data->envcp[i]);
		i++;
	}
	return (0);
}
