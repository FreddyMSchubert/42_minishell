/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 21:06:24 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
/*
	If no arguments are given, cd changes to the user's temp directory.
	Otherwise, chdir changes directory and getcwd gets the new path.
*/
char	*get_path(t_tok **tokens, t_data *program_data)
{
	char	*temp;

	if (tokens[1] == NULL || ft_strncmp(tokens[1]->val, "--", 2) == 0)
	{
		temp = get_envcp_var("HOME", program_data->envcp);
		if (temp)
			return (ft_strdup(temp));
		else
		{
			log_err("HOME not set", "cd", NULL);
			program_data->exit_status = 1;
			return (NULL);
		}
	}
	else if (tokens[1]->val[0] == '-')
	{
		temp = get_envcp_var("OLDPWD", program_data->envcp);
		if (temp)
			return (printf("%s\n", temp), ft_strdup(temp));
		else
			return (program_data->exit_status = 1, \
						log_err("OLDPWD not set", "cd", NULL), NULL);
	}
	else
		return (ft_strdup(tokens[1]->val));
}

int	change_directory(char *path, t_data *program_data)
{
	int	ret_val;

	ret_val = chdir(path);
	if (ret_val != 0)
	{
		log_err("error changing directory", "cd", NULL);
		program_data->exit_status = 1;
		return (errno);
	}
	return (0);
}

char	*get_current_directory(t_data *program_data)
{
	char	*buffer;

	buffer = getcwd(NULL, 0);
	if (!buffer)
	{
		log_err("getcwd failed", "cd", NULL);
		program_data->exit_status = 1;
	}
	return (buffer);
}

int	update_env_vars(t_data *program_data, char *buffer)
{
	if (set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), \
												1, program_data) == -1)
	{
		free(buffer);
		log_err("dynamic allocation error", "cd", NULL);
		program_data->exit_status = 1;
		return (-2);
	}
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
	{
		free(buffer);
		log_err("dynamic allocation error", "cd", NULL);
		program_data->exit_status = 1;
		return (-2);
	}
	return (0);
}

int	execute_cd(t_tok **tokens, t_data *program_data)
{
	char	*path;
	char	*buffer;

	path = get_path(tokens, program_data);
	gc_append_element(program_data->gc, path);
	if (!path)
		return (1);
	if (change_directory(path, program_data) != 0)
		return (1);
	buffer = get_current_directory(program_data);
	if (!buffer)
		return (-1);
	if (update_env_vars(program_data, buffer) != 0)
		return (-2);
	free(buffer);
	return (0);
}
