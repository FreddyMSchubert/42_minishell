/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:59:29 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/09 11:44:18 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void test_lexer(char *input)
{
	t_token **tokens = lexer(input);

	if (tokens != NULL)
	{
		print_tokens(tokens);

		t_bin_tree_node *tree = tok_to_bin_tree(tokens);
		print_binary_tree(tree, 0);

		for (int i = 0; tokens[i] != NULL; i++)
			free(tokens[i]);
		free(tokens);
	}
	else
	{
		printf("Lexer returned NULL. Memory allocation failure or other error.\n");
	}
}
