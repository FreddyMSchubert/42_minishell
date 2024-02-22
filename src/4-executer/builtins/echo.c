/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:48:20 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:05:59 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_echo(t_token **inputs)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 0;
	if (ft_strncmp(inputs[1]->value, "-n", 2) == 0 \
				&& inputs[1]->value[2] == '\0')
	{
		n_flag = 1;
		i = 2;
	}
	while (inputs[i] && inputs[i]->type == TOK_CMD_ARG)
	{
		if (ft_printf("%s", inputs[i]->value))
			return (-1);
		i++;
		if (inputs[i] && inputs[i]->type == TOK_CMD_ARG)
			ft_printf(" ");
	}
	if (n_flag == 0)
		if (ft_printf("\n") < 0)
			return (-1);
	return (0);
}
