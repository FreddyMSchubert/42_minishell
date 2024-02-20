/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:42:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/14 13:35:44 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// tabs are used to visualize tree, call function with 0
void print_binary_tree(t_bin_tree_node *tree, int tabs)
{
    int         counter;
    int         tabs_counter;

    if (tabs == 0)
        printf("BINARY TREE VISUALIZATION\nleft childs are red, right childs are blue.\n");
    counter = 0;
    while (tree->val[counter] != NULL)
    {
        tabs_counter = 0;
        while (tabs_counter < tabs)
        {
            printf("\t");
            tabs_counter++;
        }
        print_token(tree->val[counter]);
        counter++;
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

void print_token(t_token *token)
{
    printf("  Type: ");
	if (token->type == TOK_CMD_ARG)
		printf("Command or Argument");
	else if (token->type == TOK_S_QUOTE)
		printf("Single Quote");
	else if (token->type == TOK_D_QUOTE)
		printf("Double Quote");
	else if (token->type == TOK_VAR_EXP)
		printf("Variable Expansion");
	else if (token->type == TOK_EXIT_STAT)
		printf("Exit Status");
	else if (token->type == TOK_REDIR)
		printf("Redirection");
	else if (token->type == TOK_PIPE)
		printf("Pipe");
	else if (token->type == TOK_LOG_OP)
		printf("Logical Operator");
	else if (token->type == TOK_WILDCARD)
		printf("Wildcard");
	else if (token->type == TOK_BUILTIN)
		printf("Builtin");
	else if (token->type == TOK_CTRL_SEQ)
		printf("Control Sequence");
	else if (token->type == TOK_OPEN_BRACE)
		printf("Open Brace");
	else if (token->type == TOK_CLOSE_BRACE)
		printf("Close Brace");
	else
		printf("\x1b[32mUnknown\x1b[0m");
	printf(", \tValue: \"%s\", ignored: %d\n", token->value, token->ignored);
}

void print_tokens(t_token **tokens)
{
	int i = 0;
	printf("Tokens:\n");
	while (tokens[i] != NULL)
	{
	   print_token(tokens[i]);
		i++;
	}
}