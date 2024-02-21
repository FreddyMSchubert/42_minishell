/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:48:20 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 12:55:48 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int execute_echo(t_token **inputs)
{
	int		i;
	bool	n_flag;
	int		chars_printed;

	i = 1;
	n_flag = false;
	if (ft_strncmp(inputs[1]->value, "-n", 2) == 0 \
				&& inputs[1]->value[2] == '\0')
	{
		n_flag = true;
		i = 2;
	}
	while (inputs[i] && inputs[i]->type == TOK_CMD_ARG)
	{
		chars_printed = ft_printf("%s", inputs[i]->value);
		if (chars_printed < 0)
			return -1;
		i++;
		if (inputs[i] && inputs[i]->type == TOK_CMD_ARG)
			ft_printf(" ");
	}
	if (!n_flag)
	{
		chars_printed = ft_printf("\n");
		if (chars_printed < 0)
			return -1;
	}
	return 0;
}
