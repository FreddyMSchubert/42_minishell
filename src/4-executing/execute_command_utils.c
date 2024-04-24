/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 06:36:17 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/24 22:18:30 by niklasburch      ###   ########.fr       */
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

	if (access(command, X_OK) == 0 || ft_strncmp(command, "./", 2) == 0 || ft_strncmp(command, "/", 1) == 0)
		return (ft_strdup(command));
	while (envp && envp[0] && ft_strncmp(envp[0], "PATH", 4) != 0)
		envp++;
	if (!envp || !envp[0])
		return (NULL);
	split_paths = ft_split(envp[0], ':');
	if (split_paths == NULL)
		return (exec_error(-1), NULL);
	counter = 0;
	while (split_paths[counter])
	{
		path_temp = ft_strjoin(split_paths[counter], "/");
		path = ft_strjoin(path_temp, command);
		free (path_temp);
		if (!path || access(path, X_OK) == 0)
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
t_cmd_path	*create_cmd_struct(char	**envp, t_token	**cmd)
{
	t_cmd_path	*path;
	char		**split_cmd;
	int			cmd_i_counter;
	int			split_cmd_i_counter;
	int			token_amount;

	path = malloc(sizeof(t_cmd_path));
	if (!path)
		return (printf("malloc eror\n"), exec_error(-1), NULL);
	path->path = get_command_path(envp, cmd[0]->value);
	// printf("path->path: %s\n", path->path);
	if (!path->path)
		return (free(path), NULL);
	token_amount = get_token_arr_len(cmd);
	split_cmd = malloc(sizeof(char *) * (token_amount + 1));
	if (!split_cmd)
		return (free(path), exec_error(-1), NULL);
	split_cmd[token_amount] = NULL;
	cmd_i_counter = 0;
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

void	close_fds_loop(void)
{
	int	fd;

	fd = 3;
	while (fd < FD_SETSIZE)
	{
		if (fd != STDIN_FILENO && fd != STDOUT_FILENO)
			close(fd);
		fd++;
	}
}