/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:28:35 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/13 11:28:59 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int execute_echo(t_bin_tree_node *tree)
{
	int i;

	i = 1;
	while (tree->val[i])
	{
		ft_printf("%s", tree->val[i]->value);
		i++;
	}
	ft_printf("\n");
	return 0;
}
