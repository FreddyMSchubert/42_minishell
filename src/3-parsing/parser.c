/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:13:31 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 11:59:50 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	Checks whether the inputted token array is a substring
	If so, it returns the first token after the substring start and next nulls 
	the last token before the substring end
	returns NULL if no substring is found
*/
static t_list	*check_substring(t_list *tokens)
{
	int		depth;
	t_list	*current;
	t_list	*prev;
	t_list	*before_last_close_brace;

	depth = 0;
	current = tokens;
	before_last_close_brace = NULL;
	while (current != NULL)
	{
		if (((t_token *)current->content)->type == TOK_OPEN_BRACE)
			depth++;
		else if (((t_token *)current->content)->type == TOK_CLOSE_BRACE)
		{
			depth--;
			if (prev)
				before_last_close_brace = prev;
		}
		if (depth == 0 && current->next != NULL)
		{
			depth = -1;
			break ;
		}
		prev = current;
		current = current->next;
	}
	if (depth == 0)
	{
		if (before_last_close_brace)
			before_last_close_brace->next = NULL;
		return (tokens->next);
	}
	return (NULL);
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
	TOK_LOG_AND > TOK_LOG_OP > TOK_PIPE > TOK_REDIR
	4 in while condition because that's how many types of operators there are
*/
static int	get_dominant_operator(t_list **tokens)
{
	int		i;
	int		target_tok;
	int		depth;
	t_list	*current;

	target_tok = TOK_LOG_AND;
	(current = check_substring(*tokens));
	while (current != NULL)
	{
		if (current != *tokens)
			*tokens = current;
		else
			break ;
		(current = check_substring(*tokens));
	}
	while (target_tok > 0)
	{
		current = *tokens;
		i = 0;
		depth = 0;
		while (current != NULL)
		{
			if (((t_token *)current->content)->type == TOK_OPEN_BRACE)
				depth++;
			else if (((t_token *)current->content)->type == TOK_CLOSE_BRACE)
				depth--;
			if (((t_token *)current->content)->type == target_tok && \
							depth == 0 && i >= 0 && i < toklen(*tokens))
				return (i);
			i++;
			current = current->next;
		}
		target_tok = adjust_operator_priority(target_tok);
	}
	return (-1);
}

/*
	Gets a token array as input. Returns a functional binary tree structure.
	dom_op_i = dominant operator index
*/
t_bin_tree_node	*tok_to_bin_tree(t_list *tokens, t_program_data *program_data)
{
	t_bin_tree_node		*node;
	int					dom_op_i;
	t_token				**arr;

	node = malloc(sizeof(t_bin_tree_node));
	if (!node || !tokens)
		return (free(node), NULL);
	gc_append_element(program_data->gc, node);
	node->input_fd = STDIN_FILENO;
	node->output_fd = STDOUT_FILENO;
	node->redirected = false;
	dom_op_i = get_dominant_operator(&tokens);
	if (dom_op_i == -1)
	{
		arr = t_list_to_token_arr(tokens, program_data);
		return (node->val = arr, node->l = NULL, node->r = NULL, node);
	}
	node->val = malloc(sizeof(t_token) * 2);
	if (!node->val)
		return (free(node->val), free(node), NULL);
	gc_append_element(program_data->gc, node->val);
	node->val[0] = get_token_at_index(tokens, dom_op_i);
	node->val[1] = NULL;
	node->l = tok_to_bin_tree(sub_token_t_list(tokens, 0, dom_op_i - 1, program_data), program_data);
	if (node->l)
		node->l->parent = node;
	node->r = tok_to_bin_tree(sub_token_t_list(tokens, dom_op_i + 1, toklen(tokens) - 1, program_data), program_data);
	if (node->r)
		node->r->parent = node;
	return (node);
}
