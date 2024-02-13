/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:38:34 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/13 11:46:12 by fschuber         ###   ########.fr       */
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
		exit_error("Problem splitting command paths.\n");
	counter = 0;
	while (split_paths[counter])
	{
		path_temp = ft_strjoin(split_paths[counter], "/");
		path = ft_strjoin(path_temp, command);
		free (path_temp);
		if (!path || access(path, F_OK) == 0)
			return (pex_free_rec((void **)split_paths), path);
		free (path);
		counter++;
	}
	pex_free_rec((void **)split_paths);
	exit_error("ERROR finding command path.\n");
	return (NULL);
}

static int execute_builtin(t_bin_tree_node *tree)
{
	if (ft_strncmp("echo", tree->val[0]->value, 4) == 0 && 
		tree->val[0]->value[4] == '\0')
		return execute_echo(tree);
	if (ft_strncmp("cd", tree->val[0]->value, 2) == 0 && 
		tree->val[0]->value[2] == '\0')
		// return execute_cd(tree);
	if (ft_strncmp("pwd", tree->val[0]->value, 3) == 0 &&
		tree->val[0]->value[3] == '\0')
		// return execute_pwd(tree);
	if (ft_strncmp("export", tree->val[0]->value, 6) == 0 &&
		tree->val[0]->value[6] == '\0')
		// return execute_export(tree);
	if (ft_strncmp("unset", tree->val[0]->value, 5) == 0 &&
		tree->val[0]->value[5] == '\0')
		// return execute_unset(tree);
	if (ft_strncmp("env", tree->val[0]->value, 3) == 0 &&
		tree->val[0]->value[3] == '\0')
		// return execute_env(tree);
	if (ft_strncmp("exit", tree->val[0]->value, 4) == 0 &&
		tree->val[0]->value[4] == '\0')
		// return execute_exit(tree);
	return -1; // no builtin found
}

// this assumes expander already did its job
static int execute_command(t_bin_tree_node *tree)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		exit_error("ERROR: Creating of pipe failed.\n");
	pid = fork();
	if (pid == 0)
		children_routine(pipefd, create_cmd_struct(envp, cmd), in_fd, out_fd);
	else if (pid > 0)
		return (parent_routine(pid, pipefd, out_fd));
	else
		exit_error("ERROR: Forking failed.\n");
	return (0);
}

int executer(t_bin_tree_node *tree)
{
	// if the children are NULL, then it's a command
	// if the children are not NULL, then it's a logical operator, pipe, or redirection
	if (tree->l == NULL && tree->r == NULL)
	{
		// execute command
	}
	else if (tree->val[0]->type == TOK_PIPE)
		execute_pipe(tree);
	else if (tree->val[0]->type == TOK_REDIR)
		execute_redir(tree);
	else if (tree->val[0]->type == TOK_LOG_OP)
		execute_log_op(tree);
	else
		printf("Error: Invalid tree node.\n");
}
