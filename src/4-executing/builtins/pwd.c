/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 08:44:06 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/18 22:25:48 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_pwd(int out_fd, t_program_data *program_data)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		builtin_err("pwd", -4, NULL);
		return (errno);
	}
	ft_putendl_fd(pwd, out_fd);
	free(pwd);
	program_data->exit_status = 0;
	return (0);
}
