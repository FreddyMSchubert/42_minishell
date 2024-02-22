/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:39:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 13:06:04 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void broadcast_validator_error(int error, char *arg)
{
	ft_printf("error: %d, arg: %s\n", error, arg);
	ft_putstr_fd("crash: ", STDERR_FILENO);
	if (error == 1)
	{
		ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
		if (arg)
			ft_putstr_fd(arg, STDERR_FILENO);
		else
			ft_putstr_fd("(null)", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	else if (error == 2)
		ft_putstr_fd("unclosed braces\n", STDERR_FILENO);
	else if (error == 3)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": file error\n", STDERR_FILENO);
	}
	else if (error == 5)
		ft_putstr_fd("allocation error\n", STDERR_FILENO);
	
	// (void) error;
	// (void) arg;
}
