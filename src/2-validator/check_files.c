/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:16:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 13:22:38 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/// @brief checks the file linked list for errors. 0 infile, 1 outfile, 2 
///	outfile append
int	check_files(t_list *files, int flag)
{
	t_list	*tmp;

	if (files == NULL)
		return (0);
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
