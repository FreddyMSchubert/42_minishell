/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:40:52 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:34:01 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_env(t_data *sh, int out_fd)
{
	int	i;

	i = 0;
	while (sh->envcp[i])
	{
		ft_putstr_fd(sh->envcp[i], out_fd);
		ft_putchar_fd('\n', out_fd);
		i++;
	}
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0);
}
