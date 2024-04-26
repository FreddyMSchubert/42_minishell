/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 12:16:20 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 18:30:35 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @brief expansion of ~ to the home directory
void	handle_tilde_expansion(t_exp *exp, t_data *data)
{
	char	*home;

	home = get_envcp("HOME", data);
	if (!home)
		exit_error("malloc failed", 1, data->gc);
	append_to_buffer(exp, home);
	exp->buffer[exp->buf_pos] = '\0';
	free(home);
}

/// @brief expansion of the exit status
void	handle_dollar_question_expansion(t_exp *exp, t_data *data)
{
	char	*exit_status;

	exit_status = ft_itoa(data->exit_status);
	if (!exit_status)
		exit_error("malloc failed", 1, data->gc);
	append_to_buffer(exp, exit_status);
	exp->i++;
	free(exit_status);
}

/// @brief expansion of a quoted string
void	handle_dollar_quote_expansion(t_exp *exp)
{
	char	*to_append;

	to_append = ft_substr(&exp->str[exp->i], 1, \
	find_closing_quote(&exp->str[exp->i + 1], &exp->i));
	append_to_buffer(exp, to_append);
	free(to_append);
}

/// @brief expands env variables
void	handle_dollar_expansion(t_exp *exp, t_data *data)
{
	char	*env_var;
	char	*envcp_value;

	env_var = isolate_var(ft_strdup(ft_strchr(&exp->str[exp->i], '$') + 1));
	if (!env_var)
		exit_error("malloc failed", 1, data->gc);
	gc_append_element(data->gc, env_var);
	exp->i += ft_strlen(env_var);
	envcp_value = get_envcp(env_var, data);
	if (!envcp_value)
		exit_error("malloc failed", 1, data->gc);
	gc_append_element(data->gc, envcp_value);
	envcp_value = quote_operators(envcp_value);
	append_to_buffer(exp, envcp_value);
	gc_append_element(data->gc, envcp_value);
}
