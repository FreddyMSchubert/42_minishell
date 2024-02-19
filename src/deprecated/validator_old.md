/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_old.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 14:58:48 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/18 17:29:49 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
5		// Redirections (<, >, >>, <<)
6		// Pipe (|)
7		// Logical Operators (&&, ||)


error codes:
0 for no error
1 for consecutive operators
2 for unclosed braces
3 for file error
5 for allocation error
*/

/// @brief recursively checks the token tree for errors
/// @return different error codes  if an error is found, 0 if no error is found
int	check_token_tree_rec(t_bin_tree_node *node, int brace_opened, t_list **infiles, t_list **outfiles[2])
{
	int		error_code;
	t_list	*tmp;

	tmp = NULL;
	// check for consecutive operators
	if ((*node->val)->type >= 5 && (*node->val)->type <= 7)
	{
		if (((*node->l->val)->type >= 5 && (*node->l->val)->type <= 7) || \
			((*node->r->val)->type >= 5 && (*node->r->val)->type <= 7))
			return (1);
	}
	// check for valid delim for here_doc
	if ((*node->val)->type == TOK_REDIR && ft_strncmp((*node->val)->value, "<<", 2) == 0 && \
		(*node->r->val)->type != TOK_CMD_ARG && (*node->r->val)->type != TOK_VAR_EXP && \
		(*node->r->val)->type != TOK_S_QUOTE && (*node->r->val)->type != TOK_D_QUOTE && \
		(*node->r->val)->type != TOK_WILDCARD && (*node->r->val)->type != TOK_BUILTIN && \
		(*node->r->val)->type != TOK_OPEN_BRACE && (*node->r->val)->type != TOK_CLOSE_BRACE)
		return (1);

	//protect malloc???
	// put in files into linked lists
	if ((*node->val)->type == TOK_REDIR && ft_strncmp((*node->val)->value, "<", 1) == 0 && (*node->r->val)->value)
	{
		tmp = ft_lstnew((*node->r->val)->value);
		if (!tmp)
			return (5);
		ft_lstadd_back(infiles, tmp);
		tmp = NULL;
	}
	// put output files into linked lists
	if ((*node->val)->type == TOK_REDIR && ft_strncmp((*node->val)->value, ">", 1) == 0 && (*node->r->val)->value)
	{
		tmp = ft_lstnew((*node->r->val)->value);
		if (!tmp)
			return (5);
		ft_lstadd_back(outfiles[0], tmp);
		tmp = NULL;
	}
	// put append files into linked lists
	if ((*node->val)->type == TOK_REDIR && ft_strncmp((*node->val)->value, ">>", 2) == 0 && (*node->r->val)->value)
	{
		tmp = ft_lstnew((*node->r->val)->value);
		if (!tmp)
			return (5);
		ft_lstadd_back(outfiles[1], tmp);
		tmp = NULL;
	}

	//handle braces
	if (node->val[0]->type == TOK_OPEN_BRACE)
		brace_opened++;
	else if (node->val[0]->type == TOK_CLOSE_BRACE)
		brace_opened--;

	if (node->l)
	{
		error_code = check_token_tree_rec(node->l, brace_opened, infiles, outfiles);
		if (error_code > 0)
			return (error_code);
	}
	if (node->r)
	{
		error_code = check_token_tree_rec(node->r, brace_opened, infiles, outfiles);
		if (error_code > 0)
			return (error_code);
	}
	if (brace_opened > 0)
		return (2);
	return (0);
}

int	validator(t_bin_tree_node *head_node)
{
	// t_bin_tree_node	*node;
	t_list			*infiles;
	t_list			*outfiles[2];
	// t_list			*outfiles_append;
	int				error_code;

	infiles = NULL;
	outfiles[0] = NULL;
	outfiles[1] = NULL;
	error_code = check_token_tree_rec(head_node, 0, &infiles, (t_list ***)outfiles);

	// check files
	if (!error_code)
	{
		if (infiles)
		{
			error_code = check_files(&infiles, 0);
			ft_lstclear(&infiles, free);
		}
		if (outfiles[0])
		{
			error_code = check_files(&outfiles[0], 1);
			ft_lstclear(&outfiles[0], free);
		}
		if (outfiles[1])
		{
			error_code = check_files(&outfiles[1], 2);
			ft_lstclear(&outfiles[1], free);
		}
	}
	return (error_code);
}