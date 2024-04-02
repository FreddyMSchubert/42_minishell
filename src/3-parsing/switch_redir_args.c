/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_redir_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:28 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/02 17:35:41 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// t_list	*switch_redir_args(t_list *tokens)
// {
// 	t_list	*iterator;
// 	t_list	*tmp;
// 	t_token	*next;

// 	iterator = tokens;
// 	while (iterator && iterator->next && iterator->next->next && iterator->next->next->next)
// 	{
// 		next = iterator->next->content;
// 		if (next && next->type == TOK_REDIR)
// 		{
// 			tmp = iterator->next->next->next; // save the argument after the filename
// 			if (!tmp)
// 				break ;
// 			iterator->next->next->next = iterator->next; // move the redirection token after the argument
// 			iterator->next = tmp; // move the argument to before the redirection token
// 			break;
// 		}
// 		iterator = iterator->next;
// 	}
// 	return (tokens);
// }

t_list	*switch_redir_args(t_list *tokens)
{
	t_list	*iterator;
	t_list	*tmp;
	t_token	*next;
	// t_token	*before_redir;

	iterator = tokens;
	while (iterator)
	{
		if (!iterator->next)
			break ;
		next = iterator->next->content;
		if (!next)
			{printf("next is null\n");break ;}
		if (next->type == TOK_REDIR)
		{
			iterator = switch_redir_args(iterator);
			break ;
		}
		iterator = iterator->next;
	}
	if (next->type == TOK_REDIR)
	{
		tmp = iterator->next;						// cat > out 123
		iterator->next = iterator->next->next;		// iterator > 
		iterator->next = tmp;
	}
	return (tokens);
}

void test_redir_switch(t_list *tokens)
{
	t_list	*iterator;

	iterator = tokens;
	printf("Before switch:\n");
	while (iterator)
	{
		printf("Token: %s\n", ((t_token *)iterator->content)->value);
		iterator = iterator->next;
	}
	iterator = switch_redir_args(tokens);
	printf("After switch:\n");
	while (iterator)
	{
		printf("Token: %s\n", ((t_token *)iterator->content)->value);
		iterator = iterator->next;
	}

}
