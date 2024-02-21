/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:16:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/21 17:40:49 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void print_list(t_list *list)
{
	t_list *tmp;

	tmp = list;
	while (tmp != NULL)
	{
		printf("print_list: %s\n", tmp->content);
		tmp = tmp->next;
	}
}

/// @brief checks the file linked list for errors. 0 infile, 1 outfile, 2 outfile append
int check_files(t_list *files, int flag)
{
	t_list *tmp;

	if (files == NULL)
		return (0);
	// printf("flag: %d\n", flag);
	// print_list(files);
	tmp = files;
	errno = 0;
	while (tmp != NULL)
	{
		if (flag == 0)
			access(tmp->content, R_OK);
		else if (flag == 1)
			access(tmp->content, W_OK);
		else if (flag == 2)
			access(tmp->content, W_OK);
		if (errno == EACCES || (flag == 0 && errno == ENOENT))
			return (3);
		tmp = tmp->next;
	}
	return (0);
}
