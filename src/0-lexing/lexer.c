/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 06:54:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 13:18:31 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	IMPROVEMENTS COULD BE MADE HERE
	- move the check_same_string function to new utils file
	- make seperate freeing function for a token array
*/

/*
	- trim trailing and leading whitespace on command before computing tokens
	- problem with current system: " hi " - are the spaces in front and after hi part of the string? or there to seperate the tokens?
		- why do we split at "' at all if not for $ stuff?
		- and then, what if in the "" next to the $ there is a space?
		- beyond that current system should work i think
*/

/*
	@brief	Returns 0 if input strings are same length and have same content.
*/
static int check_same_string(char *str1, char *str2)
{
	if (ft_strlen(str1) == ft_strlen(str2) && \
			ft_strncmp(str1, str2, ft_strlen(str1)) == 0)
	{
		return (0);
	}
	return (-1);
}

/*
	@brief	Receives one token as string, returns categorized token struct
*/
static t_token *detect_token_type(char *input)
{
	t_token		*token;

	// printf("input: %s\n", input);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(input);
	if (ft_strncmp(input, "'", 1) == 0)
		token->type = TOK_S_QUOTE;
	else if (ft_strncmp(input, "\"", 1) == 0)
		token->type = TOK_D_QUOTE;
	else if (input[0] == '$' && input[1] && input[1] != '?')
		token->type = TOK_VAR_EXP;
	else if (input[0] == '$' && input[1] && input[1] == '?')
		token->type = TOK_EXIT_STAT;
	else if (check_same_string(input, "<") == 0 ||
			check_same_string(input, "<<") == 0 ||
			check_same_string(input, ">") == 0 ||
			check_same_string(input, ">>") == 0)
		token->type = TOK_REDIR;
	else if (check_same_string(input, "|") == 0)
		token->type = TOK_PIPE;
	else if (check_same_string(input, "&&") == 0 ||
			check_same_string(input, "||") == 0)
		token->type = TOK_LOG_OP;
	else if (ft_strchr(input, '*') != NULL)
		token->type = TOK_WILDCARD;
	else if (check_same_string(input, "echo") == 0 ||
			check_same_string(input, "cd") == 0 ||
			check_same_string(input, "pwd") == 0 ||
			check_same_string(input, "export") == 0 ||
			check_same_string(input, "unset") == 0 ||
			check_same_string(input, "env") == 0 ||
			check_same_string(input, "exit") == 0)
		token->type = TOK_BUILTIN;
	else if (check_same_string(input, "\x03") == 0 ||
			check_same_string(input, "\x04") == 0 ||
			check_same_string(input, "\x1A") == 0)
		token->type = TOK_CTRL_SEQ;
	else if (check_same_string(input, "(") == 0)
		token->type = TOK_OPEN_BRACE;
	else if (check_same_string(input, ")") == 0)
		token->type = TOK_CLOSE_BRACE;
	else
		token->type = TOK_CMD_ARG;
	token->ignored = 0;
	return (token);
}

// the following can be on other arguments without space in between
// ' " < << >> > | && || ;
// static int is_operator_symbol(char c)
// {
// 	if (c == '<' || //c == '\'' || c == '\"' || 
// 		c == '>' || c == '|' || c == '&' || c == '(' || c == ')')
// 		return (0);
// 	return (1);
// }

int	calc_str_len(char *input)
{
	int		i;
	int		cur_symbol;
	int		spaces;

	i = -1;
	spaces = 0;
	cur_symbol = 0;
	while (input[++i])
	{
		if ((input[i] == '\'' || input[i] == '\"') \
			&& cur_symbol != input[i])
			cur_symbol = input[i];
		else if (cur_symbol == input[i])
			cur_symbol = 0;
		if (cur_symbol >= 1 && cur_symbol <= 2 && \
			input[i] == ' ')
		{
			spaces++;
			cur_symbol = 0;
		}
		else if (cur_symbol < 3)
			cur_symbol = 1;
	}
	return (spaces = count_tokens(input) - spaces - 1);
}

/*
	@brief	Receives full input command, returns new string with spaces
			between tokens that are next to each other
*/
char *put_space_between_tokens(char *input)
{
	int		i;
	int		j;
	char	*new_input;
	int		in_quote;
	int		cur_symbol; // 0 for space, 1 for word, 2 operator, 3 for quotes

	new_input = ft_calloc((ft_strlen(input) + calc_str_len(input) + 1), 1);
	if (!new_input)
		return (NULL);
	i = 0;
	j = 0;
	cur_symbol = 0;
	in_quote = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"'))
		{
			if (!in_quote && cur_symbol != 0 && cur_symbol != 3) // Before entering a quote
				new_input[j++] = ' ';
			in_quote = !in_quote;
		}
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
		if (input[i] && is_operator_symbol(input[i], input[i + 1]) == 2)
			new_input[j++] = input[i++];
		new_input[j++] = input[i++];
	}
	return (new_input);
}

/*
	@brief	Receives full input command, returns array of categorized tokens
*/
t_token **lexer(char *input)
{
	char	**split_input;
	t_token	**tokens;
	int		token_amount;
	int		counter;

	split_input = ms_split(put_space_between_tokens(input));
	// for (int i = 0; split_input[i]; i++)
	// 	printf("split_input: %s\n", split_input[i]);
	token_amount = 0;
	while (split_input[token_amount])
		token_amount++;
	tokens = malloc(sizeof(t_token *) * (token_amount + 1));
	if (!tokens)
		return (NULL);
	counter = 0;
	while (counter < token_amount)
	{
		tokens[counter] = detect_token_type(split_input[counter]);
		if (!tokens[counter])
		{
			while (counter > 0)
			{
				free(tokens[counter]->value);
				free(tokens[counter]);
				counter--;
			}
			free(tokens);
			return (NULL);
		}
		counter++;
	}
	tokens[counter] = NULL;
	counter = 0;
	while (split_input[counter])
	{
		free(split_input[counter]);
		counter++;
	}
	free (split_input);
	return (tokens);
}
