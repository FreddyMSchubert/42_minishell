/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_redir_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:28 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/09 10:57:46 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_list *find_prev_node(t_list *head, t_list *node)
{
	t_list *prev = NULL, *curr = head;
	while (curr && curr != node)
	{
		prev = curr;
		curr = curr->next;
	}
	return prev;
}

void remove_node(t_list **head_ref, t_list *prev_node, t_list *node)
{
	if (prev_node != NULL)
		prev_node->next = node->next;
	else
		*head_ref = node->next;
}

void insert_node(t_list **head_ref, t_list *prev_node, t_list *node)
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

void move_node(t_list **head_ref, t_list *node1, t_list *node2)
{
	// printf("Switching nodes: %p and %p\n", node1, node2);
	// printf("Nodes to switch: %s and %s\n", ((t_token *)node1->content)->value, ((t_token *)node2->content)->value);
	if (node1 == node2 || *head_ref == NULL || node1 == NULL || node2 == NULL)
		return;

	t_list *prev_node1 = find_prev_node(*head_ref, node1);
	t_list *prev_node2 = find_prev_node(*head_ref, node2);

	// if (prev_node1 == NULL || prev_node2 == NULL)
	// 	return;

	remove_node(head_ref, prev_node2, node2);
	insert_node(head_ref, prev_node1, node2);
}

bool should_switch(t_list *node)
{
	// printf("%s\n", ((t_token *)node->content)->value);
	if (!node || !node->next || !node->next->next)
	{
		// printf("Node is null\n");
		return (false);
	}

	t_token *current = node->content;
	t_token *next_next = node->next->next->content;

	if (current == NULL || next_next == NULL)
	{
		// printf("Next is null\n");
		return (false);
	}
	// printf("Current: %s\nNext_next: %s\n", current->value, next_next->value);
	if (current->type == TOK_REDIR && next_next->type <= TOK_D_QUOTE)
		return (true);
	return (false);
}

t_list	*switch_redir_args(t_list *tokens)
{
	t_list	*iterator;

	// printf("Switching redir args\n");
	iterator = tokens;
	if (iterator->next)
		iterator->next = switch_redir_args(iterator->next);
	while (should_switch(iterator))
		move_node(&tokens, iterator, iterator->next->next);
	return (tokens);
}

// t_list	*test_redir_switch(t_list *tokens)
// {
// 	t_list	*iterator;

// 	iterator = tokens;
// 	printf("Before switch:\n");
// 	while (iterator)
// 	{
// 		printf("Token: %s\n", ((t_token *)iterator->content)->value);
// 		iterator = iterator->next;
// 	}
// 	tokens = switch_redir_args(tokens);
// 	printf("After switch:\n");
// 	iterator = tokens;
// 	while (iterator)
// 	{
// 		printf("Token: %s\n", ((t_token *)iterator->content)->value);
// 		iterator = iterator->next;
// 	}
// 	return (tokens);
// }
