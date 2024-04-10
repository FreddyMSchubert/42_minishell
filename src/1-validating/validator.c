/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:25:11 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/10 11:16:42 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
5		// Redirections (<, >, >>, <<)
6		// Pipe (|)
7		// Logical Operators (&&, ||)


error codes:
0 for no error
1 for consecutive operators
2 for unclosed braces
3 for file error
5 for allocation error
*/

// static int	add_file_to_list(t_list **files, char *value, int *error_code)
// {
// 	t_list	*tmp;

// 	// printf("error_code: %s\n", value);
// 	tmp = ft_lstnew(value);
// 	if (!tmp)
// 		*error_code = 5;
// 	if (*error_code == 0)
// 		ft_lstadd_back(files, tmp);
// 	return (*error_code);
// }

int	validator(t_list *tokens)
{
	int		brace_opened;
	int		error_code;
	t_list	*files[3]; // 0 for input, 1 for output, 2 for append_out
	t_list	*tok;
	t_token	*token;

	files[0] = NULL;
	files[1] = NULL;
	files[2] = NULL;
	brace_opened = 0;
	error_code = 0;
	tok = tokens;
	while (tok != NULL)
	{
		token = tok->content;
		// printf("token_arr: %s\n", token->value);
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

		// check for valid word after < > >> <<
		// if (token->type == TOK_REDIR)
		// 	if (!tok->next || ((t_token *)tok->next->content)->type > TOK_BUILTIN)
		// 		return (3);
		// put in files into linked lists
		// if (token->type == TOK_REDIR && ft_strncmp(token->value, "<", 2) == 0 && tok->next)
		// 	if (add_file_to_list(&files[0], ((t_token *)tok->next->content)->value, &error_code) != 0)
		// 		return (error_code);
		// put append files into linked lists
		// if (token->type == TOK_REDIR && ft_strncmp(token->value, ">>", 2) == 0 && tok->next)
		// 	if (add_file_to_list(&files[2], ((t_token *)tok->next->content)->value, &error_code) != 0)
		// 		return (error_code);
		// put output files into linked lists
		// if (token->type == TOK_REDIR && ft_strncmp(token->value, ">", 2) == 0 && tok->next)
		// 	if (add_file_to_list(&files[1], ((t_token *)tok->next->content)->value, &error_code) != 0)
		// 		return (error_code);
		tok = tok->next;
	}
	// check for | in beginning or end
	if (((t_token *)tokens->content)->type == TOK_PIPE || token->type == TOK_PIPE)
		return (1);
	if (brace_opened != 0)
		return (2);
	// check for file errors
	if (check_files(files[0], 0) != 0 || check_files(files[1], 1) != 0 || check_files(files[2], 2) != 0)
		return (ft_lstclear(&files[0], NULL), ft_lstclear(&files[1], NULL), ft_lstclear(&files[2], NULL), 3);
	return (ft_lstclear(&files[0], NULL), ft_lstclear(&files[1], NULL), ft_lstclear(&files[2], NULL), 0);
}
