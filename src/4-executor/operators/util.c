/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 07:58:05 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 07:59:20 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_filename(t_node *node)
{
	t_node	*temp;

	temp = node->r;
	while (temp->val[0]->type > TOK_D_QUOTE)
		temp = temp->l;
	return (temp->val[0]->val);
}
