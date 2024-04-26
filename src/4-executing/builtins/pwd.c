/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 08:44:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:34:16 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_pwd(int out_fd, t_data *sh)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (log_err("error changing directory", "pwd", NULL), errno);
	ft_putendl_fd(pwd, out_fd);
	free(pwd);
	sh->exit_status = 0;
	return (0);
}
