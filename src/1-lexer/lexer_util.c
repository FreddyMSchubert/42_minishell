/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 06:59:51 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 18:11:42 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*returns new list node if an operator in quotes is found and changes the 
current tokens val to the operator*/

/*
	@brief	Splits the token if an operator is found in quotes.
*/
t_list	*split_token_if_operator_in_quotes(t_tok **token, t_data *data)
{
	char	*tmp;
	int		op_len;
	t_list	*node;

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

bool	is_redirect(char c)
{
	return (c == '<' || c == '>');
}

/*
	@brief	detects type of operator symbol
	@return	2 if valid operator symbol (<<, >>, ||, &&), 
			1 if single operator symbol,
			0 if not operator symbol
*/
int	is_operator_symbol(char c, char d)
{
	if ((c == '<' && d == '<') || (c == '>' && d == '>') || \
		(c == '|' && d == '|') || (c == '&' && d == '&'))
		return (2);
	else if (c == '<' || c == '>' || c == '|' || c == '&' || \
			c == '(' || c == ')')
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
