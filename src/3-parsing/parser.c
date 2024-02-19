/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:31 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/09 11:48:43 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Checks whether the inputted token array is a substring
	If so, the first and last tokens (=brackets) are set to ignored.
*/
static int	check_substring(t_token **token_arr)
{
	int	counter;
	int	depth;

	counter = 0;
	depth = 0;
	while (token_arr[counter] != NULL)
	{
		if (token_arr[counter]->type == TOK_OPEN_BRACE)
			depth++;
		else if (token_arr[counter]->type == TOK_CLOSE_BRACE)
			depth--;
		if (depth == 0 && token_arr[counter + 1] != NULL)
		{
			depth = -1;
			break ;
		}
		counter++;
	}
	if (depth == 0)
	{
		token_arr[0]->ignored = 1;
		token_arr[counter - 1]->ignored = 1;
	}
	return (TOK_LOG_OP);
}

/*
	Sister-function to get_dominant_operator,
	updates which operator is currently looked for in that function
*/
static int	adjust_operator_priority(int current_priority)
{
	if (current_priority == TOK_LOG_OP)
		return (TOK_PIPE);
	else if (current_priority == TOK_PIPE)
		return (TOK_REDIR);
	return (current_priority);
}

/*
    Finds the most dominant operator in a token array, returning its index.
    Returns -1 if there is no dominant operator in the token array.
    TOK_LOG_OP > TOK_PIPE > TOK_REDIR
	3 in while condition because that's how many types of operators there are
*/
static int	get_dominant_operator(t_token **arr)
{
	int		i;
	int		operator_counter;
	int		targeted_token_type;
	int		depth;

	targeted_token_type = check_substring(arr);
	operator_counter = 0;
	while (operator_counter < 3)
	{
		i = 0;
		depth = 0;
		while (arr[i] != NULL)
		{
			if (arr[i]->type == TOK_OPEN_BRACE && arr[i]->ignored == 0)
				depth++;
			else if (arr[i]->type == TOK_CLOSE_BRACE && arr[i]->ignored == 0)
				depth--;
			if (arr[i]->type == targeted_token_type && depth == 0 \
			    && i > 0 && i < toklen(arr) - 1)
				return (i);
			i++;
		}
		targeted_token_type = adjust_operator_priority(targeted_token_type);
		operator_counter++;
	}
	return (-1);
}

/*
    Gets a token array as input. Returns a functional binary tree structure.
	dom_op_i = dominant operator index
*/
t_bin_tree_node	*tok_to_bin_tree(t_token **arr)
{
	t_bin_tree_node		*node;
	int					dom_op_i;

	node = malloc(sizeof(t_bin_tree_node));
	if (!node || !arr)
		return (free(node), NULL);
	dom_op_i = get_dominant_operator(arr);
	if (dom_op_i == -1)
		return (node->val = arr, node->l = NULL, node->r = NULL, node);
	node->val = malloc(sizeof(t_token) * 2);
	if (!node->val)
		return (free(node), NULL);
	node->val[1] = NULL;
	node->val[0] = arr[dom_op_i];
	node->l = tok_to_bin_tree(sub_tok_arr(arr, 0, dom_op_i - 1));
	node->r = tok_to_bin_tree(sub_tok_arr(arr, dom_op_i + 1, toklen(arr) - 1));
	if (!node->l || !node->r)
	{
		free(node->l);
		free(node->r);
		free(node);
		return (NULL);
	}
	return (node);
}
