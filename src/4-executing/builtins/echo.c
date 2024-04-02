/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:48:20 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 13:22:21 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	execute_echo(t_token **inputs, int out_fd)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 0;
	if (!inputs[1])
		return (ft_putstr_fd("\n", out_fd), 0);
	if (ft_strncmp(inputs[1]->value, "-n", 3) == 0)
	{
		n_flag = 1;
		i = 2;
	}
	while (inputs[i] && inputs[i]->type == TOK_WORD)
	{
		ft_putstr_fd(inputs[i]->value, out_fd);
		i++;
		if (inputs[i] && inputs[i]->type == TOK_WORD)
			ft_putstr_fd(" ", out_fd);
	}
	if (n_flag == 0)
		ft_putstr_fd("\n", out_fd);
	return (0);
}
