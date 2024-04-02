/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:08:39 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 09:58:56 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	builtin_err(char *builtin, int error, char *arg)
{
	ft_putstr_fd("crash: ", STDERR_FILENO);
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error == -1)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("no such file or directory", STDERR_FILENO);
	}
	else if (error == -2)
		ft_putstr_fd("dynamic allocation error", STDERR_FILENO);
	else if (error == -3)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(" not set", STDERR_FILENO);
	}
	else if (error == -4)
		ft_putstr_fd("error changing directory", STDERR_FILENO);
	else if (error == -5)
		ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
