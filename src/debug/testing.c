/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:59:29 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 16:33:49 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void test_lexer(char *input, t_program_data *data)
{
	// char *input1 = "( echo \"He'll'o $USER\" && ( export | cat < input.txt > output.txt ) ) || ( echo $? && ls * && cd /home ) && echo \"Nested start\" && ( cd /tmp && ls ) && echo \"Nested end\"";
	// char *input1 = "echo \"Hello '$USER'\"";

	t_token **tokens = lexer(input, data);
	printf("token: %p\n", tokens);
	print_tokens(tokens);

	if (tokens != NULL)
	{

		// t_bin_tree_node *tree = tok_to_bin_tree(tokens);
		// print_binary_tree(tree, 0);

		for (int i = 0; tokens[i] != NULL; i++)
			free(tokens[i]);
		free(tokens);
	}
	else
	{
		printf("Lexer returned NULL. Memory allocation failure or other error.\n");
	}
	input = NULL;
}
