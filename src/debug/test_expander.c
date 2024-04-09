/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:06:24 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/09 12:54:55 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	test_expander(t_program_data *program_data, char *input)
{
	printf("input before expansion:\n");
	printf("%s\n", input);
	input = expand_values(input, program_data, false);
	printf("input after expansion:\n");
	printf("%s\n", input);
}
