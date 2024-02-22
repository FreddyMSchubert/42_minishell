/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:06:25 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute(t_bin_tree_node *tree, t_program_data *program_data)
{
	if (tree->l == NULL && tree->r == NULL)
		execute_node(tree, program_data);
	else
	{
		execute(tree->l, program_data);
		execute(tree->r, program_data);
	}
}

int	execute_node(t_bin_tree_node *node, t_program_data *program_data)
{
	program_data->exit_status = 0;
	if (node->val[0]->type == TOK_BUILTIN)
		program_data->exit_status = execute_builtin(node, program_data);
		// right? takes exit of last command?
	else
	{
		program_data->exit_status = execute_command(node, program_data);
	}
	return (program_data->exit_status);
}

int	execute_builtin(t_bin_tree_node *node, t_program_data *program_data)
{
	if (ft_strncmp(node->val[0]->value, "echo", 4) == 0)
		return (execute_echo(node->val));
	if (ft_strncmp(node->val[0]->value, "cd", 2) == 0)
		return (execute_cd(node->val, program_data));
	if (ft_strncmp(node->val[0]->value, "pwd", 3) == 0)
		return (execute_pwd());
	if (ft_strncmp(node->val[0]->value, "export", 6) == 0)
		return (execute_export(node->val, program_data));
	if (ft_strncmp(node->val[0]->value, "unset", 5) == 0)
		return (execute_unset(node->val, program_data));
	if (ft_strncmp(node->val[0]->value, "env", 3) == 0)
		return (execute_env(program_data));
	if (ft_strncmp(node->val[0]->value, "exit", 4) == 0)
		return (execute_exit(node->val, program_data));
	return (-1);
}

int	execute_command(t_bin_tree_node *node, t_program_data *program_data)
{
	pid_t			pid;
	t_cmd_path		*cmd_path;
	int				return_value;

	pid = fork();
	if (pid == 0) // child
	{
		cmd_path = create_cmd_struct(program_data->envcp, node->val);
		execve(cmd_path->path, cmd_path->args, program_data->envcp);
		return (-1); // handle error
	}
	else if (pid > 0) // parent
	{
		waitpid(pid, &return_value, 0);
		if (WIFEXITED(return_value))
			return (WEXITSTATUS(return_value));
		else
			return (-1); // child didn't exit normally e.g. terminated by signal
	}
	else
		return (-1); // handle error
	return (0);
}
