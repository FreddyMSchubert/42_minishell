/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_redir_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:28 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 17:25:28 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	@brief	used to reorder args around redirections to be in front of them
*/

static t_list	*find_prev_node(t_list *head, t_list *node)
{
	t_list	*prev;
	t_list	*curr;

	prev = NULL;
	curr = head;
	while (curr && curr != node)
	{
		prev = curr;
		curr = curr->next;
	}
	return (prev);
}

void	insert_node(t_list **head_ref, t_list *prev_node, t_list *node)
{
	if (prev_node != NULL)
	{
		node->next = prev_node->next;
		prev_node->next = node;
	}
	else
	{
		node->next = *head_ref;
		*head_ref = node;
	}
}

void	move_node(t_list **head_ref, t_list *node1, t_list *node2)
{
	t_list	*prev_node1;
	t_list	*prev_node2;

	if (node1 == node2 || *head_ref == NULL || node1 == NULL || node2 == NULL)
		return ;
	prev_node1 = find_prev_node(*head_ref, node1);
	prev_node2 = find_prev_node(*head_ref, node2);
	if (prev_node2 != NULL)
		prev_node2->next = node2->next;
	else
		*head_ref = node2->next;
	insert_node(head_ref, prev_node1, node2);
}

bool	should_switch(t_list *node)
{
	t_tok	*current;
	t_tok	*next_next;

	if (!node || !node->next || !node->next->next)
		return (false);
	current = node->content;
	next_next = node->next->next->content;
	if (current == NULL || next_next == NULL)
		return (false);
	if (current->type == TOK_REDIR && next_next->type <= TOK_D_QUOTE)
		return (true);
	return (false);
}

t_list	*switch_redir_args(t_list *tokens)
{
	t_list	*iterator;

	iterator = tokens;
	if (iterator->next)
		iterator->next = switch_redir_args(iterator->next);
	while (should_switch(iterator))
		move_node(&tokens, iterator, iterator->next->next);
	return (tokens);
}
