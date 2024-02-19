/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 12:35:02 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/19 13:18:15 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	test_validator(void)
{
	char *input = "ls |> ||> output.txt";
	// char *input[] = {
    //     "ls",
    //     "cat < input.txt",
    //     "ls > output.txt",
    //     "ls | grep txt",
    //     "grep 'main' < input.txt | sort > output.txt",
    //     "cat >> > output.txt",
    //     "| ls",
    //     "cat <",
    //     "echo 'this is a test'",
    //     "echo \"Value: $HOME\" > output.txt",
    //     "echo $HOME",
	// 	"echo hello | > output.txt"
    // };
	t_token	**tokens;
	// for (int i = 0; input[i]; i++)
	// {
	// 	printf("\x1b[31m");
	// 	printf("TEST %d:\n", i);
	// 	printf("\x1b[0m");
	// 	tokens = lexer(input[i]);
	// 	print_tokens(tokens);
	// 	printf("error code: %d\n", validator(tokens));
	// }
	tokens = lexer(input);
	print_tokens(tokens);
	// t_bin_tree_node *tree = tok_to_bin_tree(tokens);
	// print_binary_tree(tree, 0);
}
