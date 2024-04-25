/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/25 21:41:49 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//checks if a $ or wildcard should be expanded and if the token before is a redirection operator
bool should_expand(char *str, int i, char expansion_type)
{
	int		j;

	//search for redirection token before, to return false if there is one
	j = i;
	while (j > 0 && !is_operator_symbol(str[j], ' ') && !ft_isspace(str[j]))
		j--;
	while (j > 0 && ft_isspace(str[j]))
		j--;
	if (j > 0 && ft_strnstr(&str[j - 1], "<<", 2) && !is_in_quote(str, '\"', &str[j - 1]) && !is_in_quote(str, '\'', &str[j - 1]))
		return (false);
	if (expansion_type == '~' && (ft_isspace(str[i + 1]) || !str[i + 1] || str[i + 1] == '/') && (i == 0 || ft_isspace(str[i - 1]) || is_operator_symbol(str[i - 1], ' ')) && !is_in_quote(str, '\'', &str[i]) && !is_in_quote(str, '\"', &str[i]) && !is_in_quote(str, '`', &str[i]) && !is_in_quote(str, '$', &str[i]) && !is_in_quote(str, '\'', &str[i]) && !is_in_quote(str, '"', &str[i]))
		return (true);
	else if (expansion_type == '?' && ft_strnstr(&str[i], "$?", 2) != NULL && \
			!is_in_quote(str, '\'', &str[i]))
		return (true);
	else if (expansion_type == '"' && str[i] == '$' && \
			!is_in_quote(str, '\"', &str[i]) && !is_in_quote(str, '\'', \
			&str[i]) && (str[i + 1] == '\"' || str[i + 1] == '\''))
		return (true);
	else if (expansion_type == '$' && is_valid_variable(&str[i + 1]) &&\
			!is_in_quote(str, '\'', &str[i]))
		return (true);
	else if (expansion_type == 's' && !is_valid_variable(&str[i + 1]) && \
			!is_in_quote(str, '\'', &str[i]) && !is_in_quote(str, '\"', &str[i]))
		return (true);
	else if (expansion_type == '*' && !is_in_quote(str, '\'', \
			&str[i]) && !is_in_quote(str, '\"', &str[i]))
		return (true);
	return (false);
}

/// @brief 
/// @param buffer_size current size of the buffer
/// @param buffer_pos current position in the buffer
/// @param to_append string that needs to be appended to the buffer
/// @return new buffer
void append_to_buffer(char **buffer, int *buffer_size, int *buffer_pos, char *to_append)
{
	int to_append_len = ft_strlen(to_append);
	char *temp;

	// Check if the buffer is full
	if (*buffer_pos + to_append_len >= *buffer_size)
	{
		// Reallocate the buffer with a larger size
		temp = ft_calloc(*buffer_size * 2 + to_append_len, sizeof(char));
		*buffer_size = *buffer_size * 2 + to_append_len;
		if (!temp)
		{
			fprintf(stderr, "Error allocating memory\n");
			return ;
		}
		ft_strlcpy(temp, *buffer, *buffer_pos + ft_strlen(to_append) + 1);
		free(*buffer);
		*buffer = temp;
	}
	// printf("to_append: %s\nappend_to_buffer: %s\nbuffersize: %d\n", to_append, *buffer, *buffer_size);
	// Copy the string to the buffer
	ft_strlcpy(*buffer + *buffer_pos, to_append, to_append_len + 1);
	// printf("buffer: %s\n", *buffer);
	*buffer_pos += to_append_len;
}

void handle_tilde_expansion(char **buffer, int *buffer_size, int *buffer_pos, int *i, t_program_data *program_data)
{
	char	*home;

	home = get_envcp("HOME", program_data);
	if (!home)
		exit_error("malloc failed", 1, program_data->gc);
	// printf("home: %s\n", home);
	append_to_buffer(buffer, buffer_size, buffer_pos, home);
	// printf("buffer after appending home: %s\n", *buffer);
	// printf("buffer_pos: %d\n", *buffer_pos);
	(*buffer)[*buffer_pos] = '\0';
	// printf("buffer after null termination: %s\n", *buffer);
	// (*i)++;
	// printf("i: %d\n", *i);
	(void)i;
	free(home);
}

void handle_dollar_question_expansion(char **buffer, int *buffer_size, int *buffer_pos, int *i, t_program_data *program_data)
{
	char	*exit_status;

	// printf("buffer: %s\n", *buffer);
	exit_status = ft_itoa(program_data->exit_status);
	if (!exit_status)
		exit_error("malloc failed", 1, program_data->gc);
	append_to_buffer(buffer, buffer_size, buffer_pos, exit_status);
	(*i)++;
	free(exit_status);
}

void handle_dollar_quote_expansion(char **buffer, int *buffer_size, int *buffer_pos, char *str, int *i)
{
	char	*to_append;

	to_append = ft_substr(&str[*i], 1, find_closing_quote(&str[*i + 1], i));
	append_to_buffer(buffer, buffer_size, buffer_pos, to_append);
	free(to_append);
}

void handle_dollar_expansion(char **buffer, int *buffer_size, int *buffer_pos, char *str, int *i, t_program_data *program_data)
{
	char	*env_var;
	char	*envcp_value;

	env_var = isolate_var(ft_strdup(ft_strchr(&str[*i], '$') + 1));
	if (!env_var)
		exit_error("malloc failed", 1, program_data->gc);
	gc_append_element(program_data->gc, env_var);
	*i += ft_strlen(env_var);
	envcp_value = get_envcp(env_var, program_data);
	if (!envcp_value)
		exit_error("malloc failed", 1, program_data->gc);
	gc_append_element(program_data->gc, envcp_value);
	envcp_value = quote_operators(envcp_value);
	// printf("envcp_value after quoting: %s\n", envcp_value);
	append_to_buffer(buffer, buffer_size, buffer_pos, envcp_value);
	// printf("buffer: %s\n", *buffer);
	// (*buffer)[ft_strlen(*buffer)] = '\0';
	gc_append_element(program_data->gc, envcp_value);
}

void handle_wildcard_expansion(char **buffer, int *buffer_size, int *buffer_pos, char *str, int *i, t_program_data *program_data)
{
	char	*env_var;
	int index;

	// printf("str[i]: .%c.\n", str[*i]);
	index = *i - 1;
	env_var = list_matching_files(get_pattern(str, *i, program_data));
	// printf("wildcard: %s\n", env_var);
	if (env_var)
	{
		// printf("buffer: %s$\n", *buffer);
		while ((*buffer_pos) > 0 && ((str[(*buffer_pos) - 1] != ' ' && !is_operator_symbol(str[(*buffer_pos) - 1], ' ')) || is_in_quote(str, '"', &str[(*buffer_pos) - 1]) || is_in_quote(str, '\'', &str[(*buffer_pos) - 1])))
			(*buffer_pos)--;
		// (*buffer_pos)++;
		(*buffer)[(*buffer_pos)] = '\0';
		// printf("buffer: %s$\nbuffer_size %d\nbuffer_pos %d\nenv_var %s\n", *buffer, *buffer_size, *buffer_pos, env_var);
		append_to_buffer(buffer, buffer_size, buffer_pos, env_var);
		(*buffer)[index + ft_strlen(env_var) + 1] = '\0';
		free(env_var);
		while (str[*i] && (str[*i] != ' ' || is_in_quote(str, '"', &str[*i]) || is_in_quote(str, '\'', &str[*i])) && !is_operator_symbol(str[*i], ' '))
			{/*printf("str[%d]: %c\n", *i, str[*i]);*/(*i)++;}
		// if (str[*i] && (str[*i] == ' ' || is_operator_symbol(str[*i], ' ')))
		// 	append_to_buffer(buffer, buffer_size, buffer_pos, ft_substr(&str[*i], 0, 1));
	}
	else
		append_to_buffer(buffer, buffer_size, buffer_pos, "*");
}

char *expand_values(char *str, t_program_data *program_data, bool heredoc)
{
	int i = 0;
	int buffer_size = ft_strlen(str) * 2; // Allocate a buffer of size twice the length of str
	char *buffer = ft_calloc(buffer_size, sizeof(char));
	int buffer_pos = 0;
	char *temp;

	temp = ft_calloc(2, sizeof(char));
	while (str[i])
	{
		// printf("str[%d]: %c\n", i, str[i]);
		if (str[i] == '~' && should_expand(str, i, '~'))
			handle_tilde_expansion(&buffer, &buffer_size, &buffer_pos, &i, program_data);
		else if (str[i] == '$' && should_expand(str, i, '?'))
			handle_dollar_question_expansion(&buffer, &buffer_size, &buffer_pos, &i, program_data);
		else if (str[i] == '$' && should_expand(str, i, '"'))
			handle_dollar_quote_expansion(&buffer, &buffer_size, &buffer_pos, str, &i);
		else if (str[i] == '$' && should_expand(str, i, '$'))
			handle_dollar_expansion(&buffer, &buffer_size, &buffer_pos, str, &i, program_data);
		else if (str[i] == '*' && should_expand(str, i, '*') && !heredoc)
			handle_wildcard_expansion(&buffer, &buffer_size, &buffer_pos, str, &i, program_data);
		else
		{
			temp[0] = str[i];
			append_to_buffer(&buffer, &buffer_size, &buffer_pos, temp);
		}
		// printf("i: %d\nbuffer: %s\n", i, buffer);
		i++;
	}
	// printf("i: %d\n", i);
	// printf("buffer: .%s. strlen: %zu\n", buffer, ft_strlen(buffer));
	gc_append_element(program_data->gc, buffer);
	free(temp);
	return (buffer);
}