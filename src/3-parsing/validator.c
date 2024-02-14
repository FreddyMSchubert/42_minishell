/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 14:58:48 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/14 16:19:59 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
5		// Redirections (<, >, >>, <<)
6		// Pipe (|)
7		// Logical Operators (&&, ||)
*/

/// @brief recursively checks the token tree for errors
/// @return different error codes (1 for consecutive operators) if an error is found, 0 if no error is found
int	check_token_tree_rec(t_bin_tree_node *node, int brace_opened)
{
	int		error_code;

	// check for consecutive operators
	if ((*node->val)->type >= 5 && (*node->val)->type <= 7)
	{
		if (((*node->l->val)->type >= 5 && (*node->l->val)->type >= 7) || \
			(*node->r->val)->type >= 5 && (*node->r->val)->type >= 7)
			return (1);
	}
	// check for valid delim for here_doc
	if ((*node->val)->type == TOK_REDIR && ft_strncmp((*node->val)->value, "<<", 2) == 0 && \
		(*node->r->val)->type != TOK_CMD_ARG && (*node->r->val)->type != TOK_VAR_EXP && \
		(*node->r->val)->type != TOK_S_QUOTE && (*node->r->val)->type != TOK_D_QUOTE && \
		(*node->r->val)->type != TOK_WILDCARD && (*node->r->val)->type != TOK_BUILTIN && \
		(*node->r->val)->type != TOK_OPEN_BRACE && (*node->r->val)->type != TOK_CLOSE_BRACE)
		return (1);

	if (node->val[0]->type == TOK_OPEN_BRACE)
		brace_opened++;
	else if (node->val[0]->type == TOK_CLOSE_BRACE)
		brace_opened--;

	if (node->l)
	{
		error_code = check_token_tree_rec(node->l, brace_opened);
		if (error_code > 0)
			return (error_code);
	}
	if (node->r)
	{
		error_code = check_token_tree_rec(node->r, brace_opened);
		if (error_code > 0)
			return (error_code);
	}
}

int	validator(t_bin_tree_node *head_node)
{
	t_bin_tree_node	*node;

	
}