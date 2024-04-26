/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_executer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:59:29 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 22:15:03 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	turns child process into the intended command
*/
static int	execute_execve(t_node *node, t_data *sh)
{
	t_cmd_path	*cmd_path;

	cmd_path = create_cmd_struct(sh->envcp, node->val);
	if (cmd_path)
		execve(cmd_path->path, cmd_path->args, sh->envcp);
	if (cmd_path && cmd_path->path)
		free(cmd_path->path);
	if (cmd_path && cmd_path->args)
		free(cmd_path->args);
	if (cmd_path)
		free(cmd_path);
	if (errno == EACCES)
	{
		sh->exit_status = 126;
		log_err("is a directory", node->val[0]->val, 0);
	}
	else
	{
		sh->exit_status = 127;
		log_err("command not found", node->val[0]->val, 0);
	}
	child_process_exit(sh, sh->exit_status);
	return (-1);
}

/*
	@brief	dup2s the in & out fds
*/
static void	execute_child(t_node *node, t_data *sh)
{
	if (node->in_fd != STDIN_FILENO && dup2(node->in_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input redirect failed");
		if (node->out_fd != STDOUT_FILENO)
			close(node->out_fd);
		close(node->in_fd);
		exit(-1);
	}
	if (node->out_fd != STDOUT_FILENO && \
			dup2(node->out_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output redirect failed");
		if (node->in_fd != STDIN_FILENO)
			close(node->in_fd);
		close(node->out_fd);
		exit(-1);
	}
	close_fds_loop();
	execute_execve(node, sh);
}

/*
	@brief	primarily saves the childs pid && closes the fds
*/
int	execute_parent(t_node *node, t_data *sh, t_pid_list **lst, pid_t pid)
{
	add_to_pid_list(pid, lst, false);
	if (VERBOSE == 1)
		printf("child process %d: %s\n", pid, node->val[0]->val);
	close_fds(node);
	return (sh->exit_status);
}

/*
	@brief	handles forking
*/
int	execute_node(t_node *node, t_data *sh, t_pid_list **pid_list)
{
	pid_t	pid;

	if (VERBOSE == 1)
		printf("executing %s, in: %d, out: %d\n", \
						node->val[0]->val, node->in_fd, node->out_fd);
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		if (node->out_fd != STDOUT_FILENO)
			close(node->out_fd);
		if (node->in_fd != STDIN_FILENO)
			close(node->in_fd);
		return (1);
	}
	else if (pid == 0)
		execute_child(node, sh);
	else if (pid > 0)
		return (execute_parent(node, sh, pid_list, pid));
	return ("this will never occur, just to silence warning!"[0]);
}
