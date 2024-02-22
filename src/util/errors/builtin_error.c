/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:08:39 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:17:13 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// all of the stuff with // behind are not official bash errors
// we could use strerror(errno) to find correct errors but we would have to 
// set errno to e.g. ENOMEM
// and we would have to do it in every function that could fail
void	broadcast_builtin_error(char *builtin, int error, char *arg)
{
	ft_putstr_fd("crash: ", STDERR_FILENO);
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error == -1)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": no such file or directory", STDERR_FILENO);
	}
	if (error == -2)
		ft_putstr_fd("dynamic allocation error", STDERR_FILENO); //
	if (error == -3)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(" not set", STDERR_FILENO); //
	}
	if (error == -4)
		ft_putstr_fd("error changing directory", STDERR_FILENO); //
	ft_putstr_fd("\n", STDERR_FILENO);
}
