/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:48:20 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/26 09:25:32 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_echo(t_token **inputs, int cmd_start_index)
{
	int		i;
	int		n_flag;

	i = cmd_start_index + 1;
	n_flag = 0;
	if (ft_strncmp(inputs[cmd_start_index + 1]->value, "-n", 3) == 0)
	{
		n_flag = 1;
		i = cmd_start_index + 2;
	}
	while (inputs[i] && inputs[i]->type >= TOK_CMD_ARG && inputs[i]->type <= TOK_D_QUOTE)
	{
		if (!ft_printf("%s", inputs[i]->value))
			return (-1);
		i++;
		if (inputs[i] && inputs[i]->type == TOK_CMD_ARG)// && inputs[i]->type <= TOK_D_QUOTE)
			ft_printf(" ");
	}
	if (n_flag == 0)
		if (ft_printf("\n") < 0)
			return (-1);
	return (0);
}
