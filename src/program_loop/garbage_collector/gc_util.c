/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:43:39 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/14 09:52:37 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
	Logs message & exits program with exit_code,
	but not before freeing all elements in the garbage collector
*/
void	exit_error(char *message, int exit_code, t_list *gc)
{
	ft_printf("%s\n", message);
	gc_cleanup(gc);
	exit(exit_code);
}
