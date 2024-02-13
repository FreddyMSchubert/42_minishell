/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:42:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/13 11:13:19 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	&&: execute right if left is successful; 
		return error of first failing command or 0 if all successful
	||: execute right if left is not successful;
		return 0 if any succeeds, else return error of last command
*/
int execute_log_op(t_bin_tree_node *tree)
{
	int left_status;
	int right_status;

	left_status = executer(tree->l);
	if (tree->val[0]->value[0] == '&')
	{
		if (left_status == 0)
			return (executer(tree->r));
		else
			return left_status;
	}
	else
	{
		if (left_status != 0)
			return (executer(tree->r));
		else
			return 0;
	}
}

int execute_redir(t_bin_tree_node *tree)
{
	
}

int execute_pipe(t_bin_tree_node *tree)
{
	// dup2 the output of the left command to the input of the right command
}
