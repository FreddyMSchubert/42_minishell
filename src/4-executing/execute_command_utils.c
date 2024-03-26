/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 06:36:17 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/25 09:56:49 by fschuber         ###   ########.fr       */
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

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
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
	return (ft_free_rec((void **)split_paths), NULL);
}

static int	get_token_arr_len(t_token	**cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	return (i);
}

/*
	@brief	Creates a path struct consisting of a path string
	@brief	and the arguments as an array of strings
*/
t_cmd_path	*create_cmd_struct(char	**envp, t_token	**cmd, int cmd_start_index)
{
	t_cmd_path	*path;
	char		**split_cmd;
	int			cmd_i_counter;
	int			split_cmd_i_counter;
	int			token_amount;

	path = malloc(sizeof(t_cmd_path));
	if (!path)
		return (NULL); // handle error
	path->path = get_command_path(envp, cmd[cmd_start_index]->value);
	if (!path->path)
		return (free(path), NULL); // handle error
	token_amount = get_token_arr_len(cmd);
	split_cmd = malloc(sizeof(char *) * (token_amount + 1));
	if (!split_cmd)
		return (free(path), NULL); // handle error
	split_cmd[token_amount] = NULL;
	cmd_i_counter = cmd_start_index;
	split_cmd_i_counter = 0;
	while (cmd[cmd_i_counter])
	{
		split_cmd[split_cmd_i_counter] = cmd[cmd_i_counter]->value;
		split_cmd_i_counter++;
		cmd_i_counter++;
	}
	path->args = split_cmd;
	return (path);
}
