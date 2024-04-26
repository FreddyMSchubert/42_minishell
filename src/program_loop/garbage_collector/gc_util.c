/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:43:39 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:03:52 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
	@brief	exits the program with an error message & clears garbage collector
	@param	message		the error message to be displayed
	@param	exit_code	the exit code
	@param	gc			the garbage collector
*/
void	exit_error(char *message, int exit_code, t_list *gc)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	gc_cleanup(gc);
	exit(exit_code);
}
