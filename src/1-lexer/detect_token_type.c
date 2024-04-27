/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:48:11 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:35:54 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	Creates a token struct and assigns the input to the val field.
*/
t_tok	*create_token(char *input, t_data *data)
{
	t_tok	*token;

	token = malloc(sizeof(t_tok));
	if (!token)
		return (NULL);
	gc_append_element(data->gc, token);
	token->val = input;
	return (token);
}

/*
	@brief	Creates a list struct and assigns the token to the content field.
*/
t_list	*create_list(t_tok *token, t_data *data)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	gc_append_element(data->gc, list);
	list->content = token;
	return (list);
}

/*
	@brief	Checks whether a string should be treated as a builtin command.
*/
static int	is_builtin_string(char *input, int is_first_or_after_operator)
{
	if (is_first_or_after_operator && \
		(same_str(input, "echo") == 0 || \
		same_str(input, "cd") == 0 || \
		same_str(input, "pwd") == 0 || \
		same_str(input, "export") == 0 || \
		same_str(input, "unset") == 0 || \
		same_str(input, "env") == 0 || \
		same_str(input, "exit") == 0))
		return (0);
	return (-1);
}

/*
	@brief	Assigns a type to the token based on its value.
*/
void	assign_token_type(t_tok *token, int is_first_or_after_operator)
{
	if (in_quote(token->val, '\'', token->val))
		token->type = TOK_S_QUOTE;
	else if (in_quote(token->val, '\"', token->val))
		token->type = TOK_D_QUOTE;
	else if (ft_strnstr(token->val, "<", 1) || ft_strnstr(token->val, "<<", 2) \
		|| ft_strnstr(token->val, ">", 1) || ft_strnstr(token->val, ">>", 2))
		token->type = TOK_REDIR;
	else if (ft_strnstr(token->val, "||", 2))
		token->type = TOK_LOG_OR;
	else if (ft_strnstr(token->val, "&&", 2))
		token->type = TOK_LOG_AND;
	else if (ft_strnstr(token->val, "|", 1))
		token->type = TOK_PIPE;
	else if (is_builtin_string(token->val, is_first_or_after_operator) == 0)
		token->type = TOK_BUILTIN;
	else if (ft_strnstr(token->val, "(", 1))
		token->type = TOK_OPEN_BRACE;
	else if (ft_strnstr(token->val, ")", 1))
		token->type = TOK_CLOSE_BRACE;
	else
		token->type = TOK_WORD;
}

/*
	@brief	Parent function to token detection, handles dynamic allocation etc.
*/
t_list	*detect_token_type(char *input, int is_first_or_after_operator, t_data \
*data)
{
	t_tok		*token;
	t_list		*list;

	if (!input)
		return (NULL);
	token = create_token(input, data);
	if (!token)
		return (NULL);
	list = create_list(token, data);
	if (!list)
		return (free(token), NULL);
	assign_token_type(token, is_first_or_after_operator);
	list->next = split_token_if_operator_in_quotes(&token, data);
	token->val = get_rid_of_quotes(token->val);
	gc_append_element(data->gc, token->val);
	return (list);
}
