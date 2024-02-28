/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/28 14:46:07 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute(t_bin_tree_node *tree, t_program_data *program_data)//, int count_pids)
{
	if (program_data->exit_flag == 1 || !tree)
		return ;
	if (tree->l == NULL && tree->r == NULL)
		execute_node(tree, program_data);//, &count_pids);
	else
	{
		if (tree->val[0]->type == TOK_LOG_OP)
			logical_op(tree, program_data);
		else if (tree->val[0]->type == TOK_PIPE)
			setup_pipe(tree, program_data);
		// this is just temporary so everything runs through
		// if (tree->l != NULL)
			execute(tree->l, program_data);//, count_pids);
		// if (tree->r != NULL)
			execute(tree->r, program_data);//, count_pids);
	}
}

int	execute_node(t_bin_tree_node *node, t_program_data *program_data)//, int *count_pids)
{
	int	cmd_start_index;
	pid_t	pid;
	int	return_value;

	cmd_start_index = 0;
	while (node->val[cmd_start_index]->ignored == 1)
		cmd_start_index++;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (-1);
	}
	else if (pid == 0) // child
	{
		if (node->input_fd != STDIN_FILENO)
		{
			if (dup2(node->input_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 input redirect failed");
				return (-1);
			}
			close(node->input_fd);
		}
		if (node->output_fd != STDOUT_FILENO)
		{
			if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 output redirect failed");
				return (-1);
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
			// *count_pids += 1;
		}
		exit(program_data->exit_status);
	}
	else if (pid > 0) // parent
	{
		if (node->output_fd != STDOUT_FILENO)
			close(node->output_fd);
		if (node->input_fd != STDIN_FILENO)
			close(node->input_fd);
		// printf("parent process\n");
		waitpid(pid, &return_value, 0);
		// printf("child process exited with status: %d\n", WEXITSTATUS(return_value));
		if (WIFEXITED(return_value))
			return (WEXITSTATUS(return_value));
		else
			return (-1); // child didn't exit normally e.g. terminated by signal
	}
	// if (node->input_fd != 0 || node->output_fd != 1)
	// 	print_pipes(node->input_fd, node->output_fd);
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

	// printf("executing command: %s\n", node->val[0]->value);
		// printf("child process\ninput_fd: %d\noutput_fd: %d\n", node->input_fd,node->output_fd);
		// Redirect input if necessary
	cmd_path = create_cmd_struct(program_data->envcp, node->val,
			cmd_start_index);
	execve(cmd_path->path, cmd_path->args, program_data->envcp);
	return (-1); // handle error
	// else if (pid > 0) // parent
	// {
	// 	if (node->output_fd != STDOUT_FILENO)
	// 		close(node->output_fd);
	// 	if (node->input_fd != STDIN_FILENO)
	// 		close(node->input_fd);
	// 	// printf("parent process\n");
	// 	waitpid(pid, &return_value, 0);
	// 	// printf("child process exited with status: %d\n", WEXITSTATUS(return_value));
	// 	if (WIFEXITED(return_value))
	// 		return (WEXITSTATUS(return_value));
	// 	else
	// 		return (-1); // child didn't exit normally e.g. terminated by signal
	// }
}
