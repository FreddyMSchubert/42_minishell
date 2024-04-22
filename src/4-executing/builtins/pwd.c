/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 08:44:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/22 07:22:25 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_pwd(int out_fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
        log_err("error changing directory", "pwd", NULL);
		return (errno);
	}
	ft_putendl_fd(pwd, out_fd);
	free(pwd);
	return (0);
}
