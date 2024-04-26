/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:44:43 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:15:14 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	used for builtin leaf nodes, it calls the right builtin function
			and cleans up the nodes file descriptors
*/
static int	execute_builtin(t_node *node, t_data *sh, t_pid_list **pid_list)
{
	if (VERBOSE == 1)
		printf("executing builtin %s, in: %d, out: %d\n", \
					node->val[0]->val, node->in_fd, node->out_fd);
	if (ft_strncmp(node->val[0]->val, "echo", 4) == 0)
		sh->exit_status = execute_echo(node->val, node->out_fd, sh);
	else if (ft_strncmp(node->val[0]->val, "cd", 2) == 0)
		sh->exit_status = execute_cd(node->val, sh);
	else if (ft_strncmp(node->val[0]->val, "pwd", 3) == 0)
		sh->exit_status = execute_pwd(node->out_fd, sh);
	else if (ft_strncmp(node->val[0]->val, "export", 6) == 0)
		sh->exit_status = execute_export(node->val, node->out_fd, sh);
	else if (ft_strncmp(node->val[0]->val, "unset", 5) == 0)
		sh->exit_status = execute_unset(node->val, sh);
	else if (ft_strncmp(node->val[0]->val, "env", 3) == 0)
		sh->exit_status = execute_env(sh, node->out_fd);
	else if (ft_strncmp(node->val[0]->val, "exit", 4) == 0)
		sh->exit_status = execute_exit(node->val, sh, node->out_fd);
	if (node->out_fd != STDOUT_FILENO)
		close(node->out_fd);
	if (node->in_fd != STDIN_FILENO)
		close(node->in_fd);
	add_to_pid_list(-1, pid_list, true);
	if (VERBOSE == 1)
		printf("closing fds: %d, %d\n", node->in_fd, node->out_fd);
	return (sh->exit_status);
}

/*
	@brief		determines whether node is a builtin or not
*/
static void	execute_leaf(t_node *tree, t_data *sh, t_pid_list **pid_list)
{
	if (tree->val[0]->type != TOK_BUILTIN)
		sh->exit_status = execute_node(tree, sh, pid_list);
	else
		sh->exit_status = execute_builtin(tree, sh, pid_list);
}

/*
	@brief		Logic for moving through the tree
*/
static pid_t	execute_branch(t_node *tree, t_data *sh, t_pid_list **pid_list)
{
	if (tree->val[0]->type == TOK_LOG_AND)
		return (logical_and(tree, sh));
	else if (tree->val[0]->type == TOK_LOG_OR)
		return (logical_or(tree, sh));
	else if (tree->val[0]->type == TOK_PIPE)
		setup_pipe(tree);
	else if (tree->val[0]->type == TOK_REDIR)
		if (redirect(tree, sh) == 1 || !tree->l)
			return (sh->exit_status);
	if (tree->val[0]->type == TOK_REDIR && tree->r->val[0]->type == TOK_REDIR \
			&& ((tree->l && tree->l->val[0]->type < tree->r->val[0]->type) \
			|| (tree->parent && tree->parent->val[0]->type == TOK_REDIR)))
		if (redirect(tree->r, sh) == 1)
			return (sh->exit_status);
	sh->exit_status = execute(tree->l, sh, pid_list);
	if (tree->val[0]->type == TOK_PIPE && tree->r->redirected == false)
		sh->exit_status = execute(tree->r, sh, pid_list);
	return (sh->exit_status);
}

/*
	@brief		determines whether inputted node is a leaf or a branch node
*/
pid_t	execute(t_node *tree, t_data *sh, t_pid_list **pid_list)
{
	if (sh->exit_flag == 1 || !tree)
		return (sh->exit_status);
	if (tree->l == NULL && tree->r == NULL)
		execute_leaf(tree, sh, pid_list);
	execute_branch(tree, sh, pid_list);
	return (sh->exit_status);
}
