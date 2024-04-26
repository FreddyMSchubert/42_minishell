/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 06:54:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/24 18:46:46 by niklasburch      ###   ########.fr       */
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
static t_list	*detect_token_type(char *input, int is_first_or_after_operator, t_data *program_data)
{
	t_tok		*token;
	t_list		*list;

	if (!input)
		return (NULL);
	list = malloc(sizeof(t_list));
	token = malloc(sizeof(t_tok));
	if (!token || !list)
		return (free(token), free(list), NULL);
	gc_append_element(program_data->gc, token);
	gc_append_element(program_data->gc, list);
	token->val = input;
	if (is_in_quote(token->val, '\'', input))
		token->type = TOK_S_QUOTE;
	else if (is_in_quote(token->val, '\"', input))
		token->type = TOK_D_QUOTE;
	else if (ft_strnstr(token->val, "<", 1) || ft_strnstr(token->val, "<<", 2) || \
			ft_strnstr(token->val, ">", 1) || ft_strnstr(token->val, ">>", 2))
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
	// if (token->type == TOK_S_QUOTE || token->type == TOK_D_QUOTE)
	// 	token->type = TOK_WORD;
	list->next = split_token_if_operator_in_quotes(&token, program_data);
	list->content = token;
	token->val = get_rid_of_quotes(token->val);
	gc_append_element(program_data->gc, token->val);
	return (list);
}

//returns new list node if an operator in quotes is found and changes the current tokens val to the operator
t_list	*split_token_if_operator_in_quotes(t_tok **token, t_data *data)
{
	char	*tmp;
	t_list	*node;
	int		op_len;


	node = NULL;
	op_len = is_operator_symbol((*token)->val[0], (*token)->val[1]);
	if ((*token)->type > TOK_BUILTIN && (int)ft_strlen((*token)->val) > op_len)
	{
		tmp = ft_substr((*token)->val, 0, op_len);
		gc_append_element(data->gc, tmp);
		node = detect_token_type((*token)->val + op_len, 0, data);
		(*token)->val = tmp;
	}
	return (node);
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
	if (count_tokens(input) == -1 || count_tokens(input) - spaces - 1 < 0)
		return (0);
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

	new_input is to be freed by ms_split
*/
static char	*put_space_between_tokens(char *input, t_data *data)
{
	int		i;
	int		j;
	char	*new_input;
	int		cur_symbol;
	int		in_quote;

	if (count_tokens(input) == -1)
		exit_error("Syntax error", 258, data->gc);
	new_input = ft_calloc((ft_strlen(input) + calc_add_spaces(input) + 1), 1);
	if (!new_input)
		return (NULL);
	// printf("calc_add_spaces: %d\n", calc_add_spaces(input));
	gc_append_element(data->gc, new_input);
	i = 0;
	j = 0;
	cur_symbol = SYM_SPC;
	in_quote = 0;
	// printf("input: %s\n", input);
	while (input[i] && j < (int)ft_strlen(input) + calc_add_spaces(input))
	{
		// printf("input[%d]: %c\n", i, input[i]);
		if ((input[i] == '\'' && in_quote != '\'') || (input[i] == '\"' && in_quote != '"'))
			in_quote = input[i];
		else if (in_quote == input[i])
			in_quote = 0;
		else if (!in_quote && is_operator_symbol(input[i], input[i + 1]) > 0)
		{
			if (cur_symbol != 0)
				new_input[j++] = ' ';
			cur_symbol = 2;
		}
		else if (input[i] == ' ')
			cur_symbol = 0;
		else if (input[i] != ' ' && !in_quote)
		{
			if (cur_symbol == 2 || (cur_symbol == 1 && is_operator_symbol(input[i], input[i + 1]) > 0))
				new_input[j++] = ' ';
			cur_symbol = 1;
		}
		else if (input[i] != ' ')
			cur_symbol = 1;
		if (input[i] && is_operator_symbol(input[i], input[i + 1]) == 2)
			new_input[j++] = input[i++];
		if (input[i])
			new_input[j++] = input[i++];
	}
	// printf("new_input: %s\n", new_input);
	return (new_input);
}

/*
	@brief	Receives full input command, returns array of categorized tokens
*/
t_list	*lex(char *input, t_data *data)
{
	char	**split_input;
	t_list	*tokens;
	int		token_amount;
	int		counter;
	int		is_first_or_after_operator;

	split_input = ms_split(put_space_between_tokens(input, data), data);
	if (!split_input)
		return (NULL);
	token_amount = 0;
	while (split_input && split_input[token_amount])
		token_amount++;
	counter = 0;
	tokens = NULL;
	while (counter < token_amount)
	{
		is_first_or_after_operator = 0;
		if (counter == 0 || (is_operator_symbol(split_input[counter - 1][0], \
				split_input[counter - 1][1]) != 0 && !is_redirect(*split_input[counter - 1])))
			is_first_or_after_operator = 1;
		ft_lstadd_back(&tokens, detect_token_type(split_input[counter], is_first_or_after_operator, data));
		if (ft_lstlast(tokens)->content == NULL)
			return (NULL);
		counter++;
	}
	return (tokens);
}
