/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 06:36:17 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/26 09:23:32 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief		Returns a dynamically allocated path to the command.
	@brief		It's on the caller to free the path.
	@brief		Expects cleaned up command, so no flags, params, etc
*/
static char	*get_command_path(char **envp, char *command)
{
	char	**split_paths;
	char	*path;
	char	*path_temp;
	int		counter;

	while (ft_strncmp(envp[0], "PATH", 4) != 0)
		envp++;
	split_paths = ft_split(envp[0], ':');
	if (split_paths == NULL)
		return (NULL); // handle error
	counter = 0;
	while (split_paths[counter])
	{
		path_temp = ft_strjoin(split_paths[counter], "/");
		path = ft_strjoin(path_temp, command);
		free (path_temp);
		if (!path || access(path, F_OK) == 0)
			return (ft_free_rec((void **)split_paths), path);
		free (path);
		counter++;
	}
	ft_free_rec((void **)split_paths);
	return (NULL);
}

/*
	@brief	Creates a path struct consisting of a path string
	@brief	and the arguments as an array of strings
*/
t_cmd_path	*create_cmd_struct(char	**envp, t_token	**cmd, int cmd_start_index)
{
	t_cmd_path	*path;
	char		**split_cmd;
	int			counter;

	path = malloc(sizeof(t_cmd_path));
	if (!path)
		return (NULL); // handle error
	path->path = get_command_path(envp, cmd[cmd_start_index]->value);
	counter = 1;
	while (cmd[counter])
		counter++;
	split_cmd = malloc(sizeof(char *) * (counter - cmd_start_index + 1));
	if (!split_cmd)
		return (NULL); // handle error
	split_cmd[counter - cmd_start_index] = NULL;
	counter = cmd_start_index;
	while (cmd[counter])
	{
		split_cmd[counter - cmd_start_index] = cmd[counter]->value;
		counter++;
	}
	path->args = split_cmd;
	return (path);
}
