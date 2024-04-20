/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/20 19:46:22 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
/*
	If no arguments are given, cd changes to the user's temp directory.
	Otherwise, chdir changes directory and getcwd gets the new path.
*/
int	execute_cd(t_token **tokens, t_program_data *program_data)
{
	char	*path;
	char	*buffer;
	char	*temp;
	int		ret_val;

	if (tokens[1] == NULL || ft_strncmp(tokens[1]->value, "--", 2) == 0)
	{
		temp = get_envcp_var("HOME", program_data->envcp);
		if (temp)
			path = ft_strdup(temp);
		else
			return (builtin_err("cd", -3, "HOME"), 1);
	}
	else if (tokens[1]->value[0] == '-')
	{
		temp = get_envcp_var("OLDPWD", program_data->envcp);
		if (temp)
			path = ft_strdup(temp);
		if (!temp || !path)
			return (builtin_err("cd", -3, "OLDPWD"), 1);
		printf("%s\n", path);
	}
	else
		path = ft_strdup(tokens[1]->value);
	ret_val = chdir(path);
	if (ret_val != 0)
		return (builtin_err("cd", -4, NULL), errno);
	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (builtin_err("cd", -5, "getcwd failed"), -1);
	free(path);
	program_data->exit_status = 0;
	if (set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), 1, program_data) == -1)
		return (free(buffer), builtin_err("cd", -2, "OLDPWD"), -2);
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
		return (free(buffer), builtin_err("cd", -2, "PWD"), -2);
	return (free(buffer), 0);
}
