/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 09:55:57 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:33:32 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	child_process_exit(t_data *sh, int exitcode)
{
	int			i;

	gc_cleanup(sh->gc);
	i = -1;
	while (sh->envcp[++i])
		free(sh->envcp[i]);
	free(sh->envcp);
	exit(exitcode);
}
