/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/07 11:07:05 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/wait.h>
#include <unistd.h>

pid_t	execute(t_bin_tree_node *tree, t_program_data *program_data)
{
	int	last_pid;

	last_pid = -1;
	// leaves
	if (program_data->exit_flag == 1 || !tree)
		return (program_data->exit_status);
	if (tree->l == NULL && tree->r == NULL)
		last_pid = execute_node(tree, program_data);
	else
	{
	// branches
		if (tree->val[0]->type == TOK_LOG_OP)
			last_pid = logical_op(tree, program_data);
		else if (tree->val[0]->type == TOK_PIPE)
			setup_pipe(tree, program_data);
		else if (tree->val[0]->type == TOK_REDIR)
			redirect(tree, program_data);
		if (tree->l->val[0]->type < tree->r->val[0]->type && tree->r->val[0]->type == TOK_REDIR)
			redirect(tree->r, program_data);
		if (tree->val[0]->type == TOK_REDIR || tree->val[0]->type == TOK_PIPE)
		{
		    last_pid = execute(tree->l, program_data);
            last_pid = execute(tree->r, program_data);
		}
	}
	return (last_pid);
}

int	execute_node(t_bin_tree_node *node, t_program_data *program_data)
{
	int	cmd_start_index;
	pid_t	pid;
	int	return_value;

	// printf("node_exec: %s, in_fd: %d, out_fd: %d\n", node->val[0]->value, node->input_fd, node->output_fd);
	cmd_start_index = 0;
	while (node->val[cmd_start_index]->ignored == 1)
		cmd_start_index++;
	if (node->val[cmd_start_index]->type == TOK_BUILTIN && ft_strncmp(node->val[cmd_start_index]->value, "exit", 5) == 0 && node->input_fd == 0 && node->output_fd == 1)
		return (execute_exit(node->val, program_data, cmd_start_index));
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(node->input_fd);
		close(node->output_fd);
		return (-1);
	}
	else if (pid == 0) // child
	{
		if (node->input_fd != STDIN_FILENO)
		{
			if (dup2(node->input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input redirect failed");
				close(node->input_fd);
				close(node->output_fd);
				exit(-1);
			}
			close(node->input_fd);
		}
		if (node->output_fd != STDOUT_FILENO)
		{
			if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output redirect failed");
				close(node->input_fd);
				close(node->output_fd);
				exit(-1);
			}
			close(node->output_fd);
		}
		if (node->val[cmd_start_index]->type == TOK_BUILTIN)
			program_data->exit_status = execute_builtin(node, program_data,
					cmd_start_index);
		else
		{
			program_data->exit_status = execute_command(node, program_data,
					cmd_start_index);
		}
		exit(program_data->exit_status);
	}
	else if (pid > 0) // parent
	{
		if (node->output_fd != STDOUT_FILENO)
			close(node->output_fd);
		if (node->input_fd != STDIN_FILENO)
			close(node->input_fd);
       	if (WIFEXITED(return_value))
            program_data->exit_status = WEXITSTATUS(return_value);
        else
      		program_data->exit_status = -1;
	}
	return (program_data->exit_status);
}

int	execute_builtin(t_bin_tree_node *node, t_program_data *program_data,
		int cmd_start_index)
{
	if (ft_strncmp(node->val[cmd_start_index]->value, "echo", 4) == 0)
		return (execute_echo(node->val, cmd_start_index));
	if (ft_strncmp(node->val[cmd_start_index]->value, "cd", 2) == 0)
		return (execute_cd(node->val, program_data, cmd_start_index));
	if (ft_strncmp(node->val[cmd_start_index]->value, "pwd", 3) == 0)
		return (execute_pwd());
	if (ft_strncmp(node->val[cmd_start_index]->value, "export", 6) == 0)
		return (execute_export(node->val, program_data, cmd_start_index));
	if (ft_strncmp(node->val[cmd_start_index]->value, "unset", 5) == 0)
		return (execute_unset(node->val, program_data, cmd_start_index));
	if (ft_strncmp(node->val[cmd_start_index]->value, "env", 3) == 0)
		return (execute_env(program_data));
	if (ft_strncmp(node->val[cmd_start_index]->value, "exit", 4) == 0)
		return (execute_exit(node->val, program_data, cmd_start_index));
	return (-1);
}

int	execute_command(t_bin_tree_node *node, t_program_data *program_data,
		int cmd_start_index)
{
	t_cmd_path	*cmd_path;
	// int			return_value;

	cmd_path = create_cmd_struct(program_data->envcp, node->val,
			cmd_start_index);
	execve(cmd_path->path, cmd_path->args, program_data->envcp);
	return (-1); // handle error
}
