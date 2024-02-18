/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 12:35:02 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/18 19:26:52 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	test_validator(void)
{
	char *input = "file1 cat | wc -l | ls >> file";
	t_token	**tokens = lexer(input);
	print_tokens(tokens);
	printf("error code: %d\n", validator(tokens));
	// t_bin_tree_node *tree = tok_to_bin_tree(tokens);
	// print_binary_tree(tree, 0);
}
