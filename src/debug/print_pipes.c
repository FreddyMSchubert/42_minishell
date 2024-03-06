/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:55:08 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/27 12:47:47 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_pipes(int in_fd, int out_fd)
{
	char	*str;

	printf("print_pipe: %d %d\n", in_fd, out_fd);
	while (1)
	{
		str = get_next_line(out_fd);
		write(1, str, 42);
		if (str == NULL)
			break ;
		free(str);
	}
}