/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/24 20:57:33 by niklasburch      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int execute_builtin(t_bin_tree_node *node, t_program_data *program_data)
{
	if (ft_strncmp(node->val[0]->value, "echo", 4) == 0)
		execute_echo(node->val, node->output_fd, program_data);
	if (ft_strncmp(node->val[0]->value, "cd", 2) == 0)
		execute_cd(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "pwd", 3) == 0)
		execute_pwd(node->output_fd, program_data);
	if (ft_strncmp(node->val[0]->value, "export", 6) == 0)
		execute_export(node->val, node->output_fd, program_data);
	if (ft_strncmp(node->val[0]->value, "unset", 5) == 0)
		execute_unset(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "env", 3) == 0)
		execute_env(program_data, node->output_fd);
	if (ft_strncmp(node->val[0]->value, "exit", 4) == 0)
		execute_exit(node->val, program_data, node->output_fd);
	if (node->output_fd != STDOUT_FILENO)
		close(node->output_fd);
	if (node->input_fd != STDIN_FILENO)
		close(node->input_fd);
	return (-1);
}

static int execute_command(t_bin_tree_node *node, t_program_data *program_data)
{
	t_cmd_path *cmd_path;
	char *error_msg;

	cmd_path = create_cmd_struct(program_data->envcp, node->val);
	if (cmd_path != NULL)
		execve(cmd_path->path, cmd_path->args, program_data->envcp);
	if (cmd_path && cmd_path->path)
		free(cmd_path->path);
	if (cmd_path && cmd_path->args)
		free(cmd_path->args);
	if (cmd_path)
		free(cmd_path);
	if (errno == EACCES)
	{
		program_data->exit_status = 126;
		error_msg = "is a directory";
	}
	else
	{
		program_data->exit_status = 127;
		error_msg = "command not found";
	}
	return (log_error(error_msg, node->val[0]->value, 0), -1);
}

int execute_node(t_bin_tree_node *node, t_program_data *program_data)
{
	pid_t pid;

	// if (node->val[0]->type == TOK_BUILTIN)
	// 	return(execute_builtin(node, program_data));
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
		close_fds_loop();
		execute_command(node, program_data);
		child_process_exit(program_data, program_data->exit_status);
	}
	else if (pid > 0) // parent
	{
		// if (VERBOSE == 1)
		// 	printf("child process %d: %s\n", pid, node->val[0]->value);
		if (node->output_fd != STDOUT_FILENO)
			close(node->output_fd);
		if (node->input_fd != STDIN_FILENO)
			close(node->input_fd);
		return (pid);
	}
	return (42); // this will never occur, just to silence warning
}

pid_t execute(t_bin_tree_node *tree, t_program_data *program_data, t_pid_list **pid_list)
{
	int last_pid;

	last_pid = -1;
	if (VERBOSE == 1)
		printf("executing %s, out_fd: %d\n", tree->val[0]->value, tree->output_fd);
	if (program_data->exit_flag == 1 || !tree)
		return (program_data->exit_status);
	if (tree->l == NULL && tree->r == NULL)
		if (tree->val[0]->type != TOK_BUILTIN)
			add_to_pid_list(execute_node(tree, program_data), pid_list);
		else
			execute_builtin(tree, program_data);
	else
	{
		// branches
		if (tree->val[0]->type == TOK_LOG_AND)
			return (logical_and(tree, program_data, pid_list));
		else if (tree->val[0]->type == TOK_LOG_OR)
			return (logical_or(tree, program_data, pid_list));
		else if (tree->val[0]->type == TOK_PIPE) // && !(tree->l->val[0]->type == TOK_REDIR && tree->l->val[0]->value[0] == '>'))
			setup_pipe(tree, program_data);
		else if (tree->val[0]->type == TOK_REDIR)
			if (redirect(tree, program_data) == 1 || !tree->l)
				return (last_pid);
		last_pid = execute(tree->l, program_data, pid_list);
		if (tree->val[0]->type == TOK_PIPE)
			last_pid = execute(tree->r, program_data, pid_list);
	}
	return (last_pid);
}
