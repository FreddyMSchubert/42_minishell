/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:25:11 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/24 17:21:44 by niklasburch      ###   ########.fr       */
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
int	validate(t_list *tokens)
{
	int		brace_opened;
	t_list	*files[3];
	t_list	*tok;
	t_tok	*token;

	files[0] = NULL;
	files[1] = NULL;
	files[2] = NULL;
	brace_opened = 0;
	tok = tokens;
	// check for inital token not being pipe or logical operator
	if (((t_tok *)tok->content)->type == TOK_PIPE || ((t_tok *)tok->content)->type == TOK_LOG_OR || \
		((t_tok *)tok->content)->type == TOK_LOG_AND)
		return (throw_syntax_error(((t_tok *)tok->content)->val), 258);
	while (tok != NULL)
	{
		token = tok->content;
		// check for consecutive pipes and or logical operators
		if (token->type >= TOK_PIPE && token->type <= TOK_LOG_AND && tok->next && \
				((t_tok *)tok->next->content)->type >= TOK_PIPE \
 && ((t_tok *)tok->next->content)->type <= TOK_LOG_AND)
			return (throw_syntax_error(token->val), 2);
		// check for unclosed braces
		if (token->type == TOK_OPEN_BRACE)
			brace_opened++;
		else if (token->type == TOK_CLOSE_BRACE)
			brace_opened--;
		if (brace_opened < 0)
			return (throw_syntax_error(")"), 2);
		if (token->type == TOK_OPEN_BRACE && tok->next && ((t_tok *)tok->next->content)->type == TOK_CLOSE_BRACE)
			return (throw_syntax_error(")"), 258);
		if (token->type == TOK_CLOSE_BRACE && tok->next && ((t_tok *)tok->next->content)->type == TOK_OPEN_BRACE)
			return (throw_syntax_error("("), 258);
		if (token->type == TOK_CLOSE_BRACE && tok->next && ((t_tok *)tok->next->content)->type <= TOK_BUILTIN)
			return (throw_syntax_error(((t_tok *)tok->next->content)->val), 258);
		// check for valid word after < > >> <<
		if (token->type == TOK_REDIR)
		{
			if (!tok->next)
				return (throw_syntax_error("newline"), 2);
			else if (((t_tok *)tok->next->content)->type > TOK_BUILTIN)
				return (throw_syntax_error(token->val), 2);
		}
		tok = tok->next;
	}
	// check for | in beginning or end
	if (token->type == TOK_PIPE || token->type == TOK_LOG_OR || token->type == TOK_LOG_AND)// || token->type == TOK_REDIR)
		return (throw_syntax_error(token->val), 2);
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
