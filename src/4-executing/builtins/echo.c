/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:48:20 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 13:30:19 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_echo(t_tok **inputs, int out_fd, t_data *program_data)
{
	int		i;
	int		j;
	int		n_flag;

	i = 1;
	n_flag = 0;
	if (!inputs[1])
		return (ft_putstr_fd("\n", out_fd), 0);
	while (inputs[i] && inputs[i]->value[0] == '-')
	{
		j = 1;
		while (inputs[i]->value[j] == 'n')
			j++;
		if (inputs[i]->value[j] != '\0' || j == 1)
			break;
		n_flag = 1;
		i++;
	}
	while (inputs[i] && inputs[i]->type <= TOK_D_QUOTE)
	{
		ft_putstr_fd(inputs[i]->value, out_fd);
		i++;
		if (inputs[i] && inputs[i]->type <= TOK_D_QUOTE)
			ft_putstr_fd(" ", out_fd);
	}
	if (n_flag == 0)
		ft_putstr_fd("\n", out_fd);
	program_data->exit_status = 0;
	return (0);
}
