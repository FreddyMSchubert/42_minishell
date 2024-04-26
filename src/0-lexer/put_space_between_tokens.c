/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_space_between_tokens.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 14:07:36 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:09:44 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	Calculates the amount of spaces which should be added by the
			put_space_between_tokens function
*/
static int	calc_add_spaces(char *input)
{
	int	i;
	int	cur_symbol;
	int	spaces;

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

/*
	@brief	Updates put_space_between_tokens states based on quotes
*/
static void	handle_quotes(char *input, int *indices, int *states)
{
	if ((input[indices[0]] == '\'' && states[1] != '\'')
		|| (input[indices[0]] == '\"' && states[1] != '\"'))
		states[1] = input[indices[0]];
	else if (states[1] == input[indices[0]])
		states[1] = 0;
}

/*
	@brief	Updates put_space_between_tokens states based on operator symbols
*/
static void	handle_operator_symbols(char *input, int *indices, char *new_input, \
		int *states)
{
	if (!states[1] && is_operator_symbol(input[indices[0]], \
		input[indices[0] + 1]) > 0)
	{
		if (states[0] != 0)
			new_input[indices[1]++] = ' ';
		states[0] = 2;
	}
	else if (input[indices[0]] == ' ')
		states[0] = 0;
	else if (input[indices[0]] != ' ' && !states[1])
	{
		if (states[0] == 2 || (states[0] == 1 \
		&& is_operator_symbol(input[indices[0]], input[indices[0] + 1]) > 0))
			new_input[indices[1]++] = ' ';
		states[0] = 1;
	}
	else if (input[indices[0]] != ' ')
		states[0] = 1;
}

/*
	@brief	Updates put_space_between_tokens states based on remaining symbols
*/
static void	handle_remaining_symbols(char *input, int *indices, char *new_input)
{
	if (input[indices[0]] && is_operator_symbol(input[indices[0]],
			input[indices[0] + 1]) == 2)
		new_input[indices[1]++] = input[indices[0]++];
	if (input[indices[0]])
		new_input[indices[1]++] = input[indices[0]++];
}

/*
	@brief	Receives full input command, returns new string with spaces
			between tokens that are next to each other
*/
char	*put_space_between_tokens(char *input, t_data *data)
{
	char	*new_input;
	int		indices[2];
	int		states[2];

	indices[0] = 0;
	indices[1] = 0;
	states[0] = 0;
	states[1] = 0;
	if (count_tokens(input) == -1)
		exit_error("Syntax error", 258, data->gc);
	new_input = ft_calloc((ft_strlen(input) + calc_add_spaces(input) + 1), 1);
	if (!new_input)
		return (NULL);
	gc_append_element(data->gc, new_input);
	while (input[indices[0]] && indices[1] < (int)ft_strlen(input) \
		+ calc_add_spaces(input))
	{
		handle_quotes(input, indices, states);
		handle_operator_symbols(input, indices, new_input, states);
		handle_remaining_symbols(input, indices, new_input);
	}
	return (new_input);
}
