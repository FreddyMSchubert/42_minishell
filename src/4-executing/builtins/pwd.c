/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 08:44:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 09:59:53 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		builtin_err("pwd", -4, NULL);
		return (errno);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}
