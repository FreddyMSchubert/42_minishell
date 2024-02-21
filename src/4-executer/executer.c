/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/21 09:55:31 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void execute(t_bin_tree_node *tree, t_program_data *program_data)
{
	if (tree->l == NULL && tree->r == NULL)
		execute_node(tree, program_data);
	else
	{
		execute(tree->l, program_data);
		execute(tree->r, program_data);
	}
}

void *execute_node(t_bin_tree_node *node, t_program_data *program_data)
{
	program_data->exit_status = 0;
	if (node->val[0]->type == TOK_BUILTIN)
		program_data->exit_status = execute_builtin(node, program_data); // right? takes exit of last command?
	return NULL;
}

int execute_builtin(t_bin_tree_node *node, t_program_data *program_data)
{
	(void)program_data;
	if (ft_strncmp(node->val[0]->value, "echo", 4) == 0) return execute_echo(node->val);
	if (ft_strncmp(node->val[0]->value, "cd", 2) == 0) return execute_cd(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "pwd", 3) == 0) return execute_pwd();
	if (ft_strncmp(node->val[0]->value, "export", 6) == 0) return execute_export(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "unset", 5) == 0) return execute_unset(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "env", 3) == 0) return execute_env(node->val, program_data);
	if (ft_strncmp(node->val[0]->value, "exit", 4) == 0) return execute_exit(node->val, program_data);
	return (0);
}
