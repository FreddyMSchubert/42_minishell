/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:31 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 10:24:45 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Sister-function to get_dominant_operator,
	updates which operator is currently looked for in that function
*/
static int	adjust_operator_priority(int current_priority)
{
	if (current_priority > TOK_REDIR)
		return (current_priority - 1);
	else
		return (0);
}

static t_list	*find_valid_substring(t_list *tokens)
{
	t_list	*current;

	current = check_substring(tokens);
	while (current != NULL)
	{
		if (current != tokens)
			tokens = current;
		else
			break ;
		current = check_substring(tokens);
	}
	return (tokens);
}

static int	adjust_depth(t_list	*c)
{
	if (((t_tok *)c->content)->type == TOK_OPEN_BRACE)
		return (1);
	else if (((t_tok *)c->content)->type == TOK_CLOSE_BRACE)
		return (-1);
	return (0);
}

/*
	Finds the most dominant operator in a token array, returning its index.
	Returns -1 if there is no dominant operator in the token array.
	TOK_LOG_AND > TOK_LOG_OP > TOK_PIPE > TOK_REDIR
	4 in while condition because that's how many types of operators there are
*/
static int	get_dominant_operator(t_list **tokens)
{
	int		i;
	int		target_tok;
	int		depth;
	t_list	*c;

	target_tok = TOK_LOG_AND;
	c = find_valid_substring(*tokens);
	while (target_tok > 0)
	{
		c = *tokens;
		i = -1;
		depth = 0;
		while (c != NULL && ++i >= -1)
		{
			depth += adjust_depth(c);
			if (((t_tok *)c->content)->type == target_tok && \
							depth == 0 && i >= 0 && i < toklen(*tokens))
				return (i);
			c = c->next;
		}
		target_tok = adjust_operator_priority(target_tok);
	}
	return (-1);
}

/*
	Gets a token array as input. Returns a functional binary tree structure.
	dom_op_i = dominant operator index
*/
t_node	*parse(t_list *tokens, t_data *sh)
{
	t_node		*node;
	int			dom_op_i;
	t_tok		**arr;

	node = create_default_node(sh);
	if (!node || !tokens)
		return (free(node), NULL);
	dom_op_i = get_dominant_operator(&tokens);
	if (dom_op_i == -1)
		return (arr = t_list_to_token_arr(tokens, sh), node->val = arr, \
					node->l = NULL, node->r = NULL, node);
	node->val = create_default_token_arr(sh);
	if (!node->val)
		return (free(node->val), free(node), NULL);
	node->val[0] = get_token_at_index(tokens, dom_op_i);
	node->l = parse(sub_list(tokens, 0, dom_op_i - 1, sh), sh);
	if (node->l)
		node->l->parent = node;
	node->r = parse(sub_list(tokens, dom_op_i + 1, toklen(tokens) - 1, sh), sh);
	if (node->r)
		node->r->parent = node;
	return (node);
}
