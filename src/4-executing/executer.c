/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/16 14:24:40 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

pid_t	execute(t_bin_tree_node *tree, t_program_data *program_data)
{
	int	last_pid;

	last_pid = -1;
	if (program_data->exit_flag == 1 || !tree)
		return (program_data->exit_status);
	if (tree->l == NULL && tree->r == NULL)
		last_pid = execute_node(tree, program_data);
	else
	{
	// branches
		if (tree->val[0]->type == TOK_LOG_AND)
			return (logical_and(tree, program_data));
		else if (tree->val[0]->type == TOK_LOG_OR)
			return (logical_or(tree, program_data));
		else if (tree->val[0]->type == TOK_PIPE)
			setup_pipe(tree, program_data);
		else if (tree->val[0]->type == TOK_REDIR)
			if (redirect(tree, program_data) == -2 || !tree->l)
				return (last_pid);
		if (tree->l && tree->l->val[0]->type < tree->r->val[0]->type && \
			tree->r->val[0]->type == TOK_REDIR)
			if (redirect(tree->r, program_data) == -2)
				return (last_pid);
		if (tree->val[0]->type == TOK_REDIR || tree->val[0]->type == TOK_PIPE)
			last_pid = execute(tree->l, program_data);
		if (tree->val[0]->type == TOK_PIPE)
			last_pid = execute(tree->r, program_data);
	}
	return (last_pid);
}

static int	execute_builtin(t_bin_tree_node *node, t_program_data *program_data)
{
	int	exit_code;

	if (ft_strncmp(node->val[0]->value, "echo", 4) == 0)
		exit_code = execute_echo(node->val, node->output_fd);
	if (ft_strncmp(node->val[0]->value, "cd", 2) == 0)
		exit_code = execute_cd(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "pwd", 3) == 0)
		exit_code = execute_pwd(node->output_fd);
	if (ft_strncmp(node->val[0]->value, "export", 6) == 0)
		exit_code = execute_export(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "unset", 5) == 0)
		exit_code = execute_unset(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "env", 3) == 0)
		exit_code = execute_env(program_data, node->output_fd);
	if (ft_strncmp(node->val[0]->value, "exit", 4) == 0)
		exit_code = execute_exit(node->val, program_data, node->output_fd);
	if (node->output_fd != STDOUT_FILENO)
		close(node->output_fd);
	if (node->input_fd != STDIN_FILENO)
		close(node->input_fd);
	return (-1);
}

static int	execute_command(t_bin_tree_node *node, t_program_data *program_data)
{
	t_cmd_path	*cmd_path;

	cmd_path = create_cmd_struct(program_data->envcp, node->val);
	if (cmd_path != NULL)
		execve(cmd_path->path, cmd_path->args, program_data->envcp);
	if (cmd_path && cmd_path->path)
		free(cmd_path->path);
	if (cmd_path && cmd_path->args)
		free(cmd_path->args);
	if (cmd_path)
		free(cmd_path);
	program_data->exit_status = 127;
	return (log_error("command not found", node->val[0]->value, 0), 127);
}


int	execute_node(t_bin_tree_node *node, t_program_data *program_data)
{
	pid_t	pid;

	if (node->val[0]->type == TOK_BUILTIN)
		return(execute_builtin(node, program_data));
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		if (node->output_fd != STDOUT_FILENO)
			close(node->output_fd);
		if (node->input_fd != STDIN_FILENO)
			close(node->input_fd);
		return (-1);
	}
	else if (pid == 0) // child
	{
		if (node->input_fd != STDIN_FILENO)
		{
			if (dup2(node->input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input redirect failed");
				if (node->output_fd != STDOUT_FILENO)
					close(node->output_fd);
				close(node->input_fd);
				exit(-1);
			}
			close(node->input_fd);
		}
		if (node->output_fd != STDOUT_FILENO)
		{
			if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output redirect failed");
				close(node->output_fd);
				if (node->input_fd != STDIN_FILENO)
					close(node->input_fd);
				exit(-1);
			}
			close(node->output_fd);
		}
		execute_command(node, program_data);
		child_process_exit(program_data, 127);
	}
	else if (pid > 0) // parent
	{
		if (VERBOSE == 1)
			ft_printf("child process %d: %s\n", pid, node->val[0]->value);
		if (node->output_fd != STDOUT_FILENO)
			close(node->output_fd);
		if (node->input_fd != STDIN_FILENO)
			close(node->input_fd);
		return (pid);
	}
	return (42);	// this will never occur, just to silence warning
}
