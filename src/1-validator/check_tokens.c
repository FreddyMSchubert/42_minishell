/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:32:44 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 13:32:51 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_token_errors(t_list *tok, t_tok *token)
{
	if (token->type >= TOK_PIPE && token->type <= TOK_LOG_AND && tok->next && \
		((t_tok *)tok->next->content)->type >= TOK_PIPE && \
		((t_tok *)tok->next->content)->type <= TOK_LOG_AND)
		return (throw_syntax_error(token->val), 2);
	if (token->type == TOK_REDIR)
	{
		if (!tok->next)
			return (throw_syntax_error("newline"), 2);
		else if (((t_tok *)tok->next->content)->type > TOK_BUILTIN)
			return (throw_syntax_error(token->val), 2);
	}
	return (0);
}

int	check_brace_errors(t_list *tok, t_tok *token, int *brace_opened)
{
	if (token->type == TOK_OPEN_BRACE)
		(*brace_opened)++;
	else if (token->type == TOK_CLOSE_BRACE)
		(*brace_opened)--;
	if (*brace_opened < 0)
		return (throw_syntax_error(")"), 2);
	if (token->type == TOK_OPEN_BRACE && tok->next && \
		((t_tok *)tok->next->content)->type == TOK_CLOSE_BRACE)
		return (throw_syntax_error(")"), 258);
	if (token->type == TOK_CLOSE_BRACE && tok->next && \
		((t_tok *)tok->next->content)->type == TOK_OPEN_BRACE)
		return (throw_syntax_error("("), 258);
	if (token->type == TOK_CLOSE_BRACE && tok->next && \
		((t_tok *)tok->next->content)->type <= TOK_BUILTIN)
		return (throw_syntax_error(((t_tok *)tok->next->content)->val), 258);
	return (0);
}

int	check_first_token(t_list *tok)
{
	if (((t_tok *)tok->content)->type == TOK_PIPE || \
		((t_tok *)tok->content)->type == TOK_LOG_OR || \
		((t_tok *)tok->content)->type == TOK_LOG_AND)
		return (throw_syntax_error(((t_tok *)tok->content)->val), 258);
	return (0);
}

int	check_last_token(t_tok *token)
{
	if (token->type == TOK_PIPE || token->type == TOK_LOG_OR || \
		token->type == TOK_LOG_AND)
		return (throw_syntax_error(token->val), 2);
	return (0);
}

int	check_braces(int brace_opened)
{
	if (brace_opened != 0)
	{
		if (brace_opened > 0)
			throw_syntax_error("(");
		else
			throw_syntax_error(")");
		return (2);
	}
	return (0);
}
