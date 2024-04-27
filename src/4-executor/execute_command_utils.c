/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 06:36:17 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:12:51 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief		Returns a dynamically allocated path to the command.
				It's on the caller to free the path.
				Expects cleaned up command, so no flags, params, etc
*/
static char	*get_command_path(char **envp, char *cmd)
{
	char	**split_paths;
	char	*path;
	int		counter;

	if (access(cmd, X_OK) == 0 || ft_strncmp(cmd, "./", 2) == 0 \
									|| ft_strncmp(cmd, "/", 1) == 0)
		return (ft_strdup(cmd));
	while (envp && envp[0] && ft_strncmp(envp[0], "PATH", 4) != 0)
		envp++;
	if (!envp || !envp[0])
		return (NULL);
	split_paths = ft_split(envp[0], ':');
	if (split_paths == NULL)
		return (log_err("dynamic allocation error", NULL, NULL), NULL);
	counter = -1;
	while (split_paths[++counter])
	{
		path = ft_strjoin(ft_strjoin(split_paths[counter], "/"), cmd);
		if (!path || access(path, X_OK) == 0)
			return (ft_free_rec((void **)split_paths), path);
		free (path);
	}
	return (ft_free_rec((void **)split_paths), NULL);
}

static int	get_token_arr_len(t_tok	**cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	return (i);
}

/*
	@brief	Creates a path struct consisting of a path string
			and the arguments as an array of strings
*/
t_cmd_path	*create_cmd_struct(char	**envp, t_tok	**cmd)
{
	t_cmd_path	*path;
	char		**split_cmd;
	int			cmd_i_counter;
	int			split_cmd_i_counter;
	int			token_amount;

	path = malloc(sizeof(t_cmd_path));
	if (!path)
		return (log_err("dynamic allocation error", NULL, NULL), NULL);
	path->path = get_command_path(envp, cmd[0]->val);
	if (!path->path)
		return (free(path), NULL);
	token_amount = get_token_arr_len(cmd);
	split_cmd = malloc(sizeof(char *) * (token_amount + 1));
	if (!split_cmd)
		return (free(path), log_err("dynamic allocation error", 0, 0), NULL);
	split_cmd[token_amount] = NULL;
	cmd_i_counter = -1;
	split_cmd_i_counter = -1;
	while (cmd[++cmd_i_counter] && ++split_cmd_i_counter >= -1)
		split_cmd[split_cmd_i_counter] = cmd[cmd_i_counter]->val;
	path->args = split_cmd;
	return (path);
}

/*
	@brief	Closes all file descriptors except for stdin and stdout
			Should only be called in the child process
*/
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
