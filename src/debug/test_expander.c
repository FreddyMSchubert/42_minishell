/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:06:24 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/21 12:05:30 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	test_expander(t_program_data *program_data, t_token **tokens)
{
	printf("Tokens before expansion:\n");
	print_tokens(tokens);
	expander(tokens, program_data);
	printf("Tokens after expansion:\n");
	print_tokens(tokens);
}
