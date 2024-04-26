/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:42:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/08 14:45:54 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// tabs are used to visualize tree, call function with 0
void	print_node_with_children(t_node *tree, int tabs)
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
	}
	if (tree->l != NULL)
	{
		printf("\x1b[31m");
        print_node_with_children(tree->l, tabs + 1);
		printf("\x1b[0m");
	}
	if (tree->r != NULL)
	{
		printf("\x1b[34m");
        print_node_with_children(tree->r, tabs + 1);
		printf("\x1b[0m");
	}
}

void	print_token(t_tok *token)
{
	if (!token)
	{
		printf("Token is NULL\n");
		return ;
	}
	printf("  Type: ");
	if (token->type == TOK_WORD)
		printf("Command or Argument");
	else if (token->type == TOK_S_QUOTE)
		printf("Single Quote");
	else if (token->type == TOK_D_QUOTE)
		printf("Double Quote");
	else if (token->type == TOK_REDIR)
		printf("Redirection");
	else if (token->type == TOK_PIPE)
		printf("Pipe");
	else if (token->type == TOK_LOG_OR)
		printf("Logical OR");
	else if (token->type == TOK_LOG_AND)
		printf("Logical AND");
	else if (token->type == TOK_BUILTIN)
		printf("Builtin");
	else if (token->type == TOK_OPEN_BRACE)
		printf("Open Brace");
	else if (token->type == TOK_CLOSE_BRACE)
		printf("Close Brace");
	printf(", \tValue: \"%s\"\n", token->val);
}

void	print_tokens(t_list *tokens)
{
	t_list	*tmp;

	printf("Tokens:\n");
	tmp = tokens;
	while (tmp != NULL)
	{
		print_token(tmp->content);
		tmp = tmp->next;
	}
}
