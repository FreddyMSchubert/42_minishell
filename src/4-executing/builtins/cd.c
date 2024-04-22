/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/22 07:21:44 by fschuber         ###   ########.fr       */
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
			return (log_err("HOME not set", "cd", NULL), 1);
	}
	else if (tokens[1]->value[0] == '-')
	{
		temp = get_envcp_var("OLDPWD", program_data->envcp);
		if (temp)
			path = ft_strdup(temp);
		if (!temp || !path)
			return (log_err("OLDPWD not set", "cd", NULL), 1);
		printf("%s\n", path);
	}
	else
		path = ft_strdup(tokens[1]->value);
	ret_val = chdir(path);
	if (ret_val != 0)
		return (log_err("cd", NULL, NULL), errno);
	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (log_err("getcwd failed", "cd", NULL), -1);
	free(path);
	if (set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), 1, program_data) == -1)
		return (free(buffer), log_err("dynamic allocation error", "cd", "OLDPWD"), -2);
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
		return (free(buffer), log_err("dynamic allocation error", "cd", "PWD"), -2);
	return (free(buffer), 0);
}
