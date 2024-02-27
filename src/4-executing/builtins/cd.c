/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/27 10:08:53 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_cd(t_token **tokens, t_program_data *program_data, int cmd_start_index)
{
	char	*path;
	char	*buffer;
	char	*home;
	int		ret_val;

	if (tokens[cmd_start_index + 1] == NULL)
	{
		home = get_envcp_var("HOME", program_data->envcp);
		if (home)
			path = ft_strdup(home);
		else
			return (-1); // handle error
	}
	else
		path = ft_strdup(tokens[cmd_start_index + 1]->value);
	ret_val = chdir(path);
	if (ret_val != 0)
	{
		broadcast_builtin_error("cd", -4, NULL);
		return (errno);
	}
	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (-1); // handle error
	free(path);
	set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), 0, program_data);
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
		// return (broadcast_builtin_error("cd", -2, "PWD"), 1);
		return (-2); // handle error
	return (0);
}
