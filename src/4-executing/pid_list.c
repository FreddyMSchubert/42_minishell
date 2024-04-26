/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:50:38 by niklasburch       #+#    #+#             */
/*   Updated: 2024/04/25 21:14:24 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint_received;

void	*wait_and_free(t_data *program_data, t_pid_list **pid_list)
{
	t_pid_list	*tmp;
	t_pid_list	*next;
	int			status;
	int			exit_status;

	g_sigint_received = 1;
	tmp = *pid_list;
	exit_status = 0;
	while (tmp)
	{
		if (!tmp->next && tmp->is_builtin)
			return (free(tmp), NULL);
		if (tmp->is_builtin)
		{
			next = tmp->next;
			free(tmp);
			tmp = next;
			continue ;
		}
		waitpid(tmp->pid, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = 128 + WTERMSIG(status);
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	if (program_data->exit_status == 0)
		program_data->exit_status = exit_status;
	g_sigint_received = 0;
	return (NULL);
}

int	add_to_pid_list(pid_t pid, t_pid_list **pidlist, bool is_builtin)
{
	t_pid_list	*new;
	t_pid_list	*tmp;

	new = (t_pid_list *)malloc(sizeof(t_pid_list));
	if (!new)
		return (1);
	new->pid = pid;
	new->is_builtin = is_builtin;
	new->next = NULL;
	if (*pidlist == NULL)
		*pidlist = new;
	else
	{
		tmp = *pidlist;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}
