/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 06:54:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/14 09:45:12 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	@brief	Receives one token as string, returns categorized token struct
*/
static t_list	*detect_token_type(char *input, int is_first_or_after_operator)
{
	t_token		*token;
	t_list		*list;

	list = malloc(sizeof(t_list));
	token = malloc(sizeof(t_token));
	if (!token || !list)
		return (free(token), free(list), NULL);
	if (same_str(input, "<") == 0 || same_str(input, "<<") == 0 || \
			same_str(input, ">") == 0 || same_str(input, ">>") == 0)
		token->type = TOK_REDIR;
	else if (same_str(input, "|") == 0)
		token->type = TOK_PIPE;
	else if (same_str(input, "&&") == 0)
		token->type = TOK_LOG_AND;
	else if (same_str(input, "||") == 0)
		token->type = TOK_LOG_OR;
	else if (is_builtin_string(input, is_first_or_after_operator) == 0)
		token->type = TOK_BUILTIN;
	else if (same_str(input, "(") == 0)
		token->type = TOK_OPEN_BRACE;
	else if (same_str(input, ")") == 0)
		token->type = TOK_CLOSE_BRACE;
	else
		token->type = TOK_WORD;
	token->value = ft_strdup(input);
	token->ignored = 0;
	list->content = token;
	list->next = NULL;
	return (list);
}

// calculates additional spaces needed
static int	calc_add_spaces(char *input)
{
	int		i;
	int		cur_symbol;
	int		spaces;

	i = -1;
	spaces = 0;
	cur_symbol = 0;
	while (input[++i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && cur_symbol != input[i])
			cur_symbol = input[i];
		else if (cur_symbol == input[i])
			cur_symbol = 0;
		if (cur_symbol >= 1 && cur_symbol <= 2 && input[i] == ' ')
		{
			spaces++;
			cur_symbol = 0;
		}
		else if (cur_symbol < 3)
			cur_symbol = 1;
	}
	if (count_tokens(input) == -1)
		return (-1);
	return (count_tokens(input) - spaces - 1);
}

#define SYM_SPC 0		// SPACE
#define SYM_WRD 1		// WORD
#define SYM_OPRTR 2		// OPERATOR
#define SYM_QUOT 3		// QUOTES

/*
	@brief	Receives full input command, returns new string with spaces
			between tokens that are next to each other
	in_quote = 0; // not in quote
	in_quote = 1; // '=1
	in_quote = 2; // "=2
*/
static char	*put_space_between_tokens(char *input, t_program_data *data)
{
	int		i;
	int		ni;
	char	*new_input;
	int		in_quote;
	int		cur_symbol;

	if (count_tokens(input) == -1)
		exit_error("Syntax error", 258, data->gc);
	new_input = ft_calloc((ft_strlen(input) + calc_add_spaces(input) + 1), 1);
	if (!new_input)
		return (NULL);
	i = 0;
	ni = 0;
	cur_symbol = SYM_SPC;
	in_quote = 0;
	while (ni < ((int)ft_strlen(input) + calc_add_spaces(input)) && input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"'))
		{
			if (!in_quote && cur_symbol != SYM_SPC) // Before entering a quote
				new_input[ni++] = ' ';
			if (!in_quote)
				in_quote = input[i] % 4 - 1;
		}
		else if (!in_quote && is_operator_symbol(input[i], input[i + 1]) > 0)
		{
			if (cur_symbol != SYM_SPC)
				new_input[ni++] = ' ';
			cur_symbol = SYM_OPRTR;
		}
		else if (input[i] == ' ')
			cur_symbol = SYM_SPC;
		else if (input[i] != ' ' && !in_quote)
		{
			if (cur_symbol == SYM_OPRTR || (cur_symbol == SYM_WRD && \
					is_operator_symbol(input[i], input[i + 1]) > 0))
				new_input[ni++] = ' ';
			cur_symbol = SYM_WRD;
		}
		if (input[i] && is_operator_symbol(input[i], input[i + 1]) == 2)
			new_input[ni++] = input[i++];
		new_input[ni++] = input[i++];
	}
	return (new_input);
}

/*
	@brief	Receives full input command, returns array of categorized tokens
*/
t_list	*lexer(char *input, t_program_data *data)
{
	char	**split_input;
	t_list	*tokens;
	int		token_amount;
	int		counter;
	int		is_first_or_after_operator;

	split_input = ms_split(put_space_between_tokens(input, data));
	if (!split_input)
		return (NULL);
	token_amount = 0;
	while (split_input[token_amount])
		token_amount++;
	counter = 0;
	tokens = NULL;
	while (counter < token_amount)
	{
		is_first_or_after_operator = 0;
		if (counter == 0 || is_operator_symbol(*split_input[counter - 1], \
							*split_input[counter]) != 0)
			is_first_or_after_operator = 1;
		ft_lstadd_back(&tokens, detect_token_type(split_input[counter], is_first_or_after_operator));
		if (ft_lstlast(tokens)->content == NULL)
			return (gc_cleanup(data->gc), NULL);
		counter++;
	}
	counter = -1;
	while (split_input[++counter])
		free(split_input[counter]);
	return (free (split_input), gc_append_t_list(data->gc, tokens), tokens);
}
