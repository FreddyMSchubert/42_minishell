/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 12:06:49 by nburchha          #+#    #+#             */
/*   Updated: 2024/02/28 12:59:42 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../../include/minishell.h"

// void	wait_for_processes(t_list *pid_list, t_program_data *program_data)
// {
// 	t_list	*tmp;

// 	tmp = pid_list;
// 	while (tmp)
// 	{
// 		waitpid((pid_t)tmp->content[0], (int *)&tmp->content[1], 0);
// 		tmp = tmp->next;
// 	}
// }