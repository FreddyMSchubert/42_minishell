/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:25:11 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/22 07:14:01 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	throw_syntax_error(char *token)
{
	int	err;

	err = STDERR_FILENO;
	ft_putstr_fd("minishell: syntax error near unexpected token `", err);
	ft_putstr_fd(token, err);
	ft_putstr_fd("'\n", err);
}

// files: 0 for input, 1 for output, 2 for append_out
int	validator(t_list *tokens)
{
	int		brace_opened;
	t_list	*files[3];
	t_list	*tok;
	t_token	*token;

	files[0] = NULL;
	files[1] = NULL;
	files[2] = NULL;
	brace_opened = 0;
	tok = tokens;
	// check for inital token not being pipe or logical operator
	if (((t_token *)tok->content)->type == TOK_PIPE || ((t_token *)tok->content)->type == TOK_LOG_OR || \
		((t_token *)tok->content)->type == TOK_LOG_AND)
		return (throw_syntax_error(((t_token *)tok->content)->value), 258);
	while (tok != NULL)
	{
		token = tok->content;
		// check for consecutive pipes and or logical operators
		if (token->type >= TOK_PIPE && token->type <= TOK_LOG_AND && tok->next && \
				((t_token *)tok->next->content)->type >= TOK_PIPE \
				&& ((t_token *)tok->next->content)->type <= TOK_LOG_AND)
			return (1);
		// check for unclosed braces
		if (token->type == TOK_OPEN_BRACE)
			brace_opened++;
		else if (token->type == TOK_CLOSE_BRACE)
			brace_opened--;
		if (brace_opened < 0)
			return (throw_syntax_error(")"), 2);
		if (token->type == TOK_OPEN_BRACE && tok->next && ((t_token *)tok->next->content)->type == TOK_CLOSE_BRACE)
			return (throw_syntax_error(")"), 258);
		if (token->type == TOK_CLOSE_BRACE && tok->next && ((t_token *)tok->next->content)->type == TOK_OPEN_BRACE)
			return (throw_syntax_error("("), 258);
		if (token->type == TOK_CLOSE_BRACE && tok->next && ((t_token *)tok->next->content)->type <= TOK_BUILTIN)
			return (throw_syntax_error(((t_token *)tok->next->content)->value), 258);
		// check for valid word after < > >> <<
		if (token->type == TOK_REDIR)
		{
			if (!tok->next)
				return (throw_syntax_error("newline"), 1);
			else if (((t_token *)tok->next->content)->type > TOK_BUILTIN)
				return (throw_syntax_error(token->value), 2);
		}
		tok = tok->next;
	}
	// check for | in beginning or end
	if (token->type == TOK_PIPE || token->type == TOK_LOG_OR || token->type == TOK_LOG_AND || token->type == TOK_REDIR)
		return (throw_syntax_error(token->value),258);
	if (brace_opened != 0)
	{
		if (brace_opened > 0)
			throw_syntax_error("(");
		else
			throw_syntax_error(")");
		return (2);
	}
	// check for file errors
	if (check_files(files[0], 0) != 0 || check_files(files[1], 1) != 0 || check_files(files[2], 2) != 0)
		return (ft_lstclear(&files[0], NULL), ft_lstclear(&files[1], NULL), ft_lstclear(&files[2], NULL), 3);
	return (ft_lstclear(&files[0], NULL), ft_lstclear(&files[1], NULL), ft_lstclear(&files[2], NULL), 0);
}
