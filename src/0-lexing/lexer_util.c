/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 06:59:51 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/22 09:17:35 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	is_redirect(char c)
{
	if (c == '<' || c == '>')
		return (true);
	return (false);
}

/// @return 2 if valid operator symbol (<<, >>, ||, &&), 1 if single operator symbol, 0 if not operator symbol
int	is_operator_symbol(char c, char d)
{
	if ((c == '<' && d == '<') || (c == '>' && d == '>') || (c == '|' && d == '|') || (c == '&' && d == '&'))
		return (2);
	else if (c == '<' || c == '>' || c == '|' || c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

/*
	@brief	Returns 0 if input strings are same length and have same content.
*/
int	same_str(char *str1, char *str2)
{
	if (ft_strlen(str1) == ft_strlen(str2) && \
			ft_strncmp(str1, str2, ft_strlen(str1)) == 0)
		return (0);
	return (-1);
}
