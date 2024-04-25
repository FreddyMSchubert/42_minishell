/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 12:15:12 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int execute_builtin(t_bin_tree_node *node, t_program_data *program_data)
{
	int	exit_status;

	exit_status = 0;
	if (VERBOSE == 1)
		printf("executing builtin %s, in: %d, out: %d\n", node->val[0]->value, node->input_fd, node->output_fd);
	if (ft_strncmp(node->val[0]->value, "echo", 4) == 0)
		exit_status = execute_echo(node->val, node->output_fd, program_data);
	else if (ft_strncmp(node->val[0]->value, "cd", 2) == 0)
		exit_status = execute_cd(node->val, program_data);
	else if (ft_strncmp(node->val[0]->value, "pwd", 3) == 0)
		exit_status = execute_pwd(node->output_fd, program_data);
	else if (ft_strncmp(node->val[0]->value, "export", 6) == 0)
		exit_status = execute_export(node->val, node->output_fd, program_data);
	else if (ft_strncmp(node->val[0]->value, "unset", 5) == 0)
		exit_status = execute_unset(node->val, program_data);
	else if (ft_strncmp(node->val[0]->value, "env", 3) == 0)
		exit_status = execute_env(program_data, node->output_fd);
	else if (ft_strncmp(node->val[0]->value, "exit", 4) == 0)
		exit_status = execute_exit(node->val, program_data, node->output_fd);
	if (node->output_fd != STDOUT_FILENO)
		close(node->output_fd);
	if (node->input_fd != STDIN_FILENO)
		close(node->input_fd);
	program_data->exit_status = exit_status;
	if (VERBOSE == 1)
		printf("closing fds: %d, %d\n", node->input_fd, node->output_fd);
	return (-1);
}

static int	execute_command(t_bin_tree_node *node, t_program_data *program_data)
{
	t_cmd_path	*cmd_path;
	char		*error_msg;

	cmd_path = create_cmd_struct(program_data->envcp, node->val);
	if (cmd_path)
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
	return (log_err(error_msg, node->val[0]->value, 0), -1);
}

int	execute_node(t_bin_tree_node *node, t_program_data *program_data)
{
	pid_t	pid;

	if (VERBOSE == 1)
		printf("executing %s, in: %d, out: %d\n", \
						node->val[0]->value, node->input_fd, node->output_fd);
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
	else if (pid == 0)
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
	else if (pid > 0)
	{
		if (VERBOSE == 1)
			printf("child process %d: %s\n", pid, node->val[0]->value);
		if (node->output_fd != STDOUT_FILENO)
		{
			close(node->output_fd);
			if (VERBOSE == 1)
				printf("%s: closed output fd %d\n", node->val[0]->value, node->output_fd);
		}
		if (node->input_fd != STDIN_FILENO)
		{
			close(node->input_fd);
			if (VERBOSE == 1)
				printf("%s: closed input fd %d\n", node->val[0]->value, node->input_fd);
		}
		return (pid);
	}
	return ("this will never occur, just to silence warning!"[0]);
}

pid_t	execute(t_bin_tree_node *tree, t_program_data *program_data, t_pid_list **pid_list)
{
	int last_pid;

	last_pid = -1;
	if (VERBOSE == 1)
		printf("executing %s, out_fd: %d\n", tree->val[0]->value, tree->output_fd);
	if (program_data->exit_flag == 1 || !tree)
		return (program_data->exit_status);
	if (tree->l == NULL && tree->r == NULL)
	{
		if (tree->val[0]->type != TOK_BUILTIN)
		{
			last_pid = execute_node(tree, program_data);
			add_to_pid_list(last_pid, pid_list, false);
		}
		else
			add_to_pid_list(execute_builtin(tree, program_data), pid_list, true);
	}
	else
	{
		// branches
		if (tree->val[0]->type == TOK_LOG_AND)
			return (logical_and(tree, program_data, pid_list));
		else if (tree->val[0]->type == TOK_LOG_OR)
			return (logical_or(tree, program_data, pid_list));
		else if (tree->val[0]->type == TOK_PIPE)
			setup_pipe(tree, program_data);
		else if (tree->val[0]->type == TOK_REDIR)
			if (redirect(tree, program_data) == 1 || !tree->l)
				return (last_pid);
		if (tree->val[0]->type == TOK_REDIR && tree->r->val[0]->type == TOK_REDIR && ((tree->l && tree->l->val[0]->type < tree->r->val[0]->type) || (tree->parent && tree->parent->val[0]->type == TOK_REDIR)))
			if (redirect(tree->r, program_data) == 1)
				return (last_pid);
		last_pid = execute(tree->l, program_data, pid_list);
		if (tree->val[0]->type == TOK_PIPE && tree->r->redirected == false)
			last_pid = execute(tree->r, program_data, pid_list);
	}
	return (last_pid);
}

//ls | cat << stop <- executen cat nicht