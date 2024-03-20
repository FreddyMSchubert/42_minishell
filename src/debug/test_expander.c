/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:06:24 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/19 12:18:36 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	test_expander(t_program_data *program_data, char *input)
{
	printf("input before expansion:\n");
	printf("%s\n", input);
	input = expand_values(input, program_data);
	printf("input after expansion:\n");
	printf("%s\n", input);
}
