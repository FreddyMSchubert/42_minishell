/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:59:29 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/07 10:17:09 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void test_lexer()
{
	char *input = "echo \"Hello $USER\" && pwd; export | cat < input.txt > output.txt || echo $?; ls *; cd /home; exit;";

	t_token **tokens = lexer(input);

	if (tokens != NULL)
	{
		print_tokens(tokens);

		printf("now printing created tree!\n");
		t_bin_tree_node *tree = convert_tokens_to_bin_tree(tokens);
		print_binary_tree(tree, 0);

		for (int i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	else
	{
		printf("Lexer returned NULL. Memory allocation failure or other error.\n");
	}
}
