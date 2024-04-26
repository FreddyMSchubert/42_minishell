/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:20:32 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 20:48:14 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @param buffer_size current size of the buffer
/// @param buf_pos current position in the buffer
/// @param to_append string that needs to be appended to the buffer
/// @return new buffer
void	append_to_buffer(t_exp *exp, char *to_append)
{
	int		to_append_len;
	char	*temp;

	to_append_len = ft_strlen(to_append);
	if (exp->buf_pos + to_append_len >= exp->buffer_size)
	{
		temp = ft_calloc(exp->buffer_size * 2 + to_append_len, sizeof(char));
		exp->buffer_size = exp->buffer_size * 2 + to_append_len;
		if (!temp)
		{
			ft_putstr_fd("Error allocating memory\n", 2);
			return ;
		}
		ft_strlcpy(temp, exp->buffer, exp->buf_pos + ft_strlen(to_append) + 1);
		free(exp->buffer);
		exp->buffer = temp;
	}
	ft_strlcpy(exp->buffer + exp->buf_pos, to_append, to_append_len + 1);
	exp->buf_pos += to_append_len;
}

void	expand_loop(t_exp *exp, t_data *data, char *temp, bool heredoc)
{
	if (exp->str[exp->i] == '~' && should_expand(exp->str, exp->i, '~'))
		handle_tilde_expansion(exp, data);
	else if (exp->str[exp->i] == '$' && should_expand(exp->str, exp->i, '?'))
		handle_dollar_question_expansion(exp, data);
	else if (exp->str[exp->i] == '$' && should_expand(exp->str, exp->i, '"'))
		handle_dollar_quote_expansion(exp);
	else if (exp->str[exp->i] == '$' && should_expand(exp->str, exp->i, '$'))
		handle_dollar_expansion(exp, data);
	else if (exp->str[exp->i] == '*' && should_expand(exp->str, exp->i, '*') && \
			!heredoc)
		handle_wildcard_expansion(exp, data);
	else
	{
		temp[0] = exp->str[exp->i];
		append_to_buffer(exp, temp);
	}
}

char	*expand(const char *str, t_data *data, bool heredoc)
{
	t_exp	exp;
	char	*temp;

	exp.i = -1;
	exp.buf_pos = 0;
	exp.buffer_size = ft_strlen(str) * 2;
	exp.buffer = ft_calloc(exp.buffer_size, sizeof(char));
	exp.str = str;
	temp = ft_calloc(2, sizeof(char));
	while (str[++exp.i])
		expand_loop(&exp, data, temp, heredoc);
	gc_append_element(data->gc, exp.buffer);
	free(temp);
	return (exp.buffer);
}
