/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:31 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/14 10:40:25 by fschuber         ###   ########.fr       */
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
	int	changes_made;

	counter = 0;
	depth = 0;
	changes_made = 0;
	while (token_arr[counter] != NULL)
	{
		if (token_arr[counter]->type == TOK_OPEN_BRACE && token_arr[counter]->ignored != 1)
			depth++;
		else if (token_arr[counter]->type == TOK_CLOSE_BRACE && token_arr[counter]->ignored != 1)
			depth--;
		if (depth == 0 && token_arr[counter + 1] != NULL && counter != last_non_ignored(token_arr) && token_arr[counter]->ignored != 1)
		{
			depth = -1;
			break ;
		}
		counter++;
	}
	if (depth == 0)
	{
		if (token_arr[first_non_ignored(token_arr)]->type == TOK_OPEN_BRACE)
		{
			token_arr[first_non_ignored(token_arr)]->ignored = 1;
			changes_made++;
		}
		if (token_arr[last_non_ignored(token_arr)]->type == TOK_CLOSE_BRACE)
		{
			changes_made++;
			token_arr[last_non_ignored(token_arr)]->ignored = 1;
		}
	}
	return (changes_made);
}

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
	int		target_tok;
	int		depth;

	target_tok = TOK_LOG_AND;
	while (check_substring(arr) != 0)
		;
	operator_counter = 0;
	while (operator_counter < 4)
	{
		i = -1;
		depth = 0;
		while (arr[++i] != NULL)
		{
			if (arr[i]->type == TOK_OPEN_BRACE && arr[i]->ignored == 0)
				depth++;
			else if (arr[i]->type == TOK_CLOSE_BRACE && arr[i]->ignored == 0)
				depth--;
			if (arr[i]->type == target_tok && depth == 0 && \
								i >= 0 && i < toklen(arr))
				return (i);
		}
		target_tok = adjust_operator_priority(target_tok);
		operator_counter++;
	}
	return (-1);
}

t_token	**switch_args_for_redir(t_token **arr)
{
	t_token	*temp;
	int		redir_index;
	int		args_start_index;

	redir_index = -1;
	while (arr && arr[++redir_index])
	{
		args_start_index = 0;
		if (arr[redir_index]->type == TOK_REDIR)
			args_start_index = redir_index + 1;
		if (args_start_index > 0 && arr[++args_start_index] && arr[args_start_index]->type == TOK_WORD)
		{
			temp = arr[args_start_index];
			arr[args_start_index] = arr[redir_index + 1];
			arr[redir_index + 1] = arr[redir_index];
			arr[redir_index] = temp;
		}
	}
	return (arr);
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
	node->input_fd = STDIN_FILENO;
	node->output_fd = STDOUT_FILENO;
	dom_op_i = get_dominant_operator(arr);
	if (dom_op_i == -1)
		return (node->val = arr, node->l = NULL, node->r = NULL, node);
	node->val = malloc(sizeof(t_token) * 2);
	if (!node->val)
		return (free(node), NULL);
	node->val[0] = arr[dom_op_i];
	node->val[1] = NULL;
	node->l = tok_to_bin_tree(sub_tok_arr(arr, 0, dom_op_i - 1));
	if (node->l)
		node->l->parent = node;
	node->r = tok_to_bin_tree(sub_tok_arr(arr, dom_op_i + 1, toklen(arr) - 1));
	if (node->r)
		node->r->parent = node;
	return (node);
}
