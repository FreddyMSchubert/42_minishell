/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:31 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/08 12:33:49 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
    Finds the most dominant operator in a token array, returning its index.
    Returns -1 if there is no dominant operator in the token array.
    TOK_LOG_OP > TOK_PIPE > TOK_REDIR
*/
static int get_dominant_operator(t_token **token_arr)
{
	int counter;
	int counter2;
	int targeted_token_type = TOK_LOG_OP;

	counter2 = 0;
	while (counter2 < 3) // amount of tokens to target
	{
		counter = 0;
		while (token_arr[counter]) {
		if (token_arr[counter]->type == targeted_token_type) {
			return (counter);
		}
		}
		if (targeted_token_type == TOK_LOG_OP)
			targeted_token_type = TOK_PIPE;
		else if (targeted_token_type == TOK_PIPE)
			targeted_token_type = TOK_REDIR;
		counter2++;
	}
	return (-1);
}

/*
    Gets a token array as input. Returns a functional binary tree structure.

    Could make this recursive prolly
*/
t_bin_tree_node *convert_tokens_to_bin_tree(t_token **token_arr)
{
	t_bin_tree_node		*tree_node;
	int					dominant_operator_index;

	if (!token_arr)
		return (NULL);
	tree_node = malloc(sizeof(t_bin_tree_node));
	if (!tree_node)
		return (NULL);
	dominant_operator_index = get_dominant_operator(token_arr);
	if (dominant_operator_index == -1)
	{
		tree_node->val = token_arr;
		tree_node->left = NULL;
		tree_node->right = NULL;
		return (tree_node);
	}
	tree_node->val = token_arr[dominant_operator_index];
	tree_node->left = convert_tokens_to_bin_tree(get_sub_token_arr(token_arr, 0, dominant_operator_index - 1));
	tree_node->right = convert_tokens_to_bin_tree(get_sub_token_arr(token_arr, dominant_operator_index + 1, get_token_arr_len(token_arr) - 1));
	if (!tree_node->left || !tree_node->right)
	{
		free(tree_node->left);
		free(tree_node->right);
		free(tree_node);
		return (NULL);
	}
	return (tree_node);
}
