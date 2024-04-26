/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_defaults.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 09:49:24 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:19:21 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	creates a default node as expected by the parser
	@param	sh	pointer to the data struct
	@return	pointer to the node
			NULL	if malloc failed
*/
t_node	*create_default_node(t_data *sh)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	gc_append_element(sh->gc, node);
	node->in_fd = STDIN_FILENO;
	node->out_fd = STDOUT_FILENO;
	node->redirected = false;
	node->val = NULL;
	node->l = NULL;
	node->r = NULL;
	return (node);
}

/*
	@brief	creates a default token array as expected by the parser
	@param	sh	pointer to the data struct
	@return	pointer to the token array
			NULL	if malloc failed
*/
t_tok	**create_default_token_arr(t_data *sh)
{
	t_tok	**arr;

	arr = malloc(sizeof(t_tok *) * 2);
	if (!arr)
		return (NULL);
	gc_append_element(sh->gc, arr);
	arr[0] = NULL;
	arr[1] = NULL;
	return (arr);
}
