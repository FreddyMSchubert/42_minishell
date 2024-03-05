/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:42:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/05 10:07:04 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// tabs are used to visualize tree, call function with 0
void	print_binary_tree(t_bin_tree_node *tree, int tabs)
{
	int			i;
	int			tabs_counter;

	if (tabs == 0)
		printf("BIN TREE VIS\nleft childs are red, right childs are blue.\n");
	i = -1;
	while (tree && tree->val && tree->val[++i] && tree->val[i] != NULL)
	{
		tabs_counter = -1;
		while (++tabs_counter < tabs)
			printf("\t");
		print_token(tree->val[i]);
		while (tabs_counter-- >= 0)
			printf("\t");
		printf("parent: %p, node: %p\n", tree->parent, tree);
	}
	if (tree->l != NULL)
	{
		printf("\x1b[31m");
		print_binary_tree(tree->l, tabs + 1);
		printf("\x1b[0m");
	}
	if (tree->r != NULL)
	{
		printf("\x1b[34m");
		print_binary_tree(tree->r, tabs + 1);
		printf("\x1b[0m");
	}
}

void	print_token(t_token *token)
{
	printf("  Type: ");
	if (token->type == TOK_CMD_ARG)
		printf("Command or Argument");
	else if (token->type == TOK_S_QUOTE)
		printf("Single Quote");
	else if (token->type == TOK_D_QUOTE)
		printf("Double Quote");
	else if (token->type == TOK_EXIT_STAT)
		printf("Exit Status");
	else if (token->type == TOK_REDIR)
		printf("Redirection");
	else if (token->type == TOK_PIPE)
		printf("Pipe");
	else if (token->type == TOK_LOG_OP)
		printf("Logical Operator");
	else if (token->type == TOK_BUILTIN)
		printf("Builtin");
	else if (token->type == TOK_CTRL_SEQ)
		printf("Control Sequence");
	else if (token->type == TOK_OPEN_BRACE)
		printf("Open Brace");
	else if (token->type == TOK_CLOSE_BRACE)
		printf("Close Brace");
	printf(", \tValue: \"%s\", ignored: %d\n", token->value, token->ignored);
}

void	print_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	printf("Tokens:\n");
	while (tokens[i] != NULL)
	{
		print_token(tokens[i]);
		i++;
	}
}
