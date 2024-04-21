/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/21 19:02:22 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
/*
	If no arguments are given, cd changes to the user's temp directory.
	Otherwise, chdir changes directory and getcwd gets the new path.
*/
char *get_path(t_token **tokens, t_program_data *program_data)
{
	char *temp;
	if (tokens[1] == NULL || ft_strncmp(tokens[1]->value, "--", 2) == 0)
	{
		temp = get_envcp_var("HOME", program_data->envcp);
		if (temp)
			return ft_strdup(temp);
		else
		{
			builtin_err("cd", -3, "HOME");
			program_data->exit_status = 1;
			return NULL;
		}
	}
	else if (tokens[1]->value[0] == '-')
	{
		temp = get_envcp_var("OLDPWD", program_data->envcp);
		if (temp)
		{
			printf("%s\n", temp);
			return ft_strdup(temp);
		}
		else
		{
			builtin_err("cd", -3, "OLDPWD");
			program_data->exit_status = 1;
			return NULL;
		}
	}
	else
		return ft_strdup(tokens[1]->value);
}

int change_directory(char *path, t_program_data *program_data)
{
	int ret_val = chdir(path);
	if (ret_val != 0)
	{
		builtin_err("cd", -4, NULL);
		program_data->exit_status = 1;
		return errno;
	}
	return 0;
}

char *get_current_directory(t_program_data *program_data)
{
	char *buffer = getcwd(NULL, 0);
	if (!buffer)
	{
		builtin_err("cd", -5, "getcwd failed");
		program_data->exit_status = 1;
	}
	return buffer;
}

int update_env_vars(t_program_data *program_data, char *buffer)
{
	if (set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), 1, program_data) == -1)
	{
		free(buffer);
		builtin_err("cd", -2, "OLDPWD");
		program_data->exit_status = 1;
		return -2;
	}
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
	{
		free(buffer);
		builtin_err("cd", -2, "PWD");
		program_data->exit_status = 1;
		return -2;
	}
	return 0;
}

int	execute_cd(t_token **tokens, t_program_data *program_data)
{
	char *path = get_path(tokens, program_data);
	if (!path)
		return 1;
	if (change_directory(path, program_data) != 0)
		return 1;
	char *buffer = get_current_directory(program_data);
	if (!buffer)
		return -1;
	free(path);
	if (update_env_vars(program_data, buffer) != 0)
		return -2;
	free(buffer);
	return 0;
}
