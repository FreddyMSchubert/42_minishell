/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 10:02:28 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 10:16:25 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	exec_error(int error)
{
	ft_putstr_fd("crash: envp: ", STDERR_FILENO);
	if (error == -1)
		ft_putstr_fd("dynamic allocation error\n", STDERR_FILENO);
	else if (error == -2)
		ft_putstr_fd("error getting command path\n", STDERR_FILENO);
	else if (error == -3)
		ft_putstr_fd("execve error\n", STDERR_FILENO);
}
