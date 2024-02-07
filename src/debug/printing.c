/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:42:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/07 08:09:06 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_tokens(t_token **tokens)
{
	int i = 0;
	printf("Tokens:\n");
	while (tokens[i] != NULL)
	{
		printf("  Type: ");
		if (tokens[i]->type == TOK_CMD_ARG)
			printf("Command or Argument");
		else if (tokens[i]->type == TOK_S_QUOTE)
			printf("Single Quote");
		else if (tokens[i]->type == TOK_D_QUOTE)
			printf("Double Quote");
		else if (tokens[i]->type == TOK_VAR_EXP)
			printf("Variable Expansion");
		else if (tokens[i]->type == TOK_EXIT_STAT)
			printf("Exit Status");
		else if (tokens[i]->type == TOK_REDIR)
			printf("Redirection");
		else if (tokens[i]->type == TOK_PIPE)
			printf("Pipe");
		else if (tokens[i]->type == TOK_LOG_OP)
			printf("Logical Operator");
		else if (tokens[i]->type == TOK_WILDCARD)
			printf("Wildcard");
		else if (tokens[i]->type == TOK_BUILTIN)
			printf("Builtin");
		else if (tokens[i]->type == TOK_CTRL_SEQ)
			printf("Control Sequence");
		else if (tokens[i]->type == TOK_SEMICOLON)
			printf("Semicolon");
		printf(", \tValue: \"%s\"\n", tokens[i]->value);
		i++;
	}
}
