/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:48:20 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:33:51 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	parse_echo_options(t_tok **inputs, int *i)
{
	int	n_flag;
	int	j;

	n_flag = 0;
	while (inputs[*i] && inputs[*i]->val[0] == '-')
	{
		j = 1;
		while (inputs[*i]->val[j] == 'n')
			j++;
		if (inputs[*i]->val[j] != '\0' || j == 1)
			break ;
		n_flag = 1;
		(*i)++;
	}
	return (n_flag);
}

int	execute_echo(t_tok **inputs, int out_fd, t_data *sh)
{
	int		i;
	int		n_flag;

	i = 1;
	if (!inputs[1])
		return (ft_putstr_fd("\n", out_fd), 0);
	n_flag = parse_echo_options(inputs, &i);
	while (inputs[i] && inputs[i]->type <= TOK_D_QUOTE)
	{
		ft_putstr_fd(inputs[i]->val, out_fd);
		i++;
		if (inputs[i] && inputs[i]->type <= TOK_D_QUOTE)
			ft_putstr_fd(" ", out_fd);
	}
	if (n_flag == 0)
		ft_putstr_fd("\n", out_fd);
	sh->exit_status = 0;
	return (0);
}
