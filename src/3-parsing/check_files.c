/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:16:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/18 19:45:59 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void print_list(t_list *list)
{
	t_list *tmp;

	tmp = list;
	while (tmp != NULL)
	{
		printf("print_list: %p\n", tmp);
		tmp = tmp->next;
	}
}

/// @brief checks the file linked list for errors. 0 infile, 1 outfile, 2 outfile append
int	check_files(t_list *files, int flag)
{
	print_list(files);
	printf("flag: %d\n", flag);
	// t_list	*tmp;
	// int		fd;

	// tmp = files;
	// while (tmp != NULL)
	// {
	// 	printf("check_file: %p\n", tmp);
	// 	if (flag == 0)
	// 	{
	// 		fd = open(tmp->content, O_RDONLY);
	// 		if (fd < 0)
	// 			return (3);
	// 		close(fd);
	// 	}
	// 	else if (flag == 1)
	// 	{
	// 		fd = open(tmp->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// 		if (fd < 0)
	// 			return (3);
	// 		close(fd);
	// 	}
	// 	else if (flag == 2)
	// 	{
	// 		fd = open(tmp->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 		if (fd < 0)
	// 			return (3);
	// 		close(fd);
	// 	}
	// 	tmp = tmp->next;
	// }
	return (0);
}