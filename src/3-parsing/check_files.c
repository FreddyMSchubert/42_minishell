/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:16:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/15 12:56:18 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


/// @brief checks the file linked list for errors. 0 infile, 1 outfile, 2 outfile append
int	check_files(t_list **files, int flag)
{
	t_list	*tmp;
	int		fd;

	tmp = *files;
	while (tmp->next)
	{
		printf("file: %s\n", (char *)tmp->content);
		if (flag == 0)
		{
			fd = open(tmp->content, O_RDONLY);
			if (fd < 0)
				return (3);
			close(fd);
		}
		else if (flag == 1)
		{
			fd = open(tmp->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (3);
			close(fd);
		}
		else if (flag == 2)
		{
			fd = open(tmp->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (3);
			close(fd);
		}
	}
	return (0);
}