/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:31 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:22:58 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	adjusts the priority of the operator
	@param	current_priority	current priority of the operator
	@return	new priority of the operator
*/
static int	adjust_operator_priority(int current_priority)
{
	if (current_priority > TOK_REDIR)
		return (current_priority - 1);
	else
		return (0);
}

/*
	@brief	loops through tokens to remove all braces on the outside
	@param	tokens	pointer to the token list
	@return	pointer to the first token after the last brace
			NULL	if there are no braces
*/
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

/*
	@brief	adjusts the depth of the token
	@param	c	pointer to the token list node
	@return	1	if the token is an open brace
			-1	if the token is a close brace
			0	otherwise
*/
static int	adjust_depth(t_list	*c)
{
	if (((t_tok *)c->content)->type == TOK_OPEN_BRACE)
		return (1);
	else if (((t_tok *)c->content)->type == TOK_CLOSE_BRACE)
		return (-1);
	return (0);
}

/*
	@brief	gets the index of the dominant operator in the token list
	@param	tokens	pointer to the token list
	@return	index of the dominant operator
			-1	if there is no dominant operator
*/
static int	get_dominant_operator(t_list **tokens)
{
	int		i;
	int		target_tok;
	int		depth;
	t_list	*c;

	target_tok = TOK_LOG_AND;
	*tokens = find_valid_substring(*tokens);
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
	@brief	parses the token list into a binary tree
	@param	tokens	pointer to the token list
	@param	sh		pointer to the data struct
	@return	pointer to the root node of the tree
			NULL	if malloc failed
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
