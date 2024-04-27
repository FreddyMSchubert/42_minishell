/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:50:38 by niklasburch       #+#    #+#             */
/*   Updated: 2024/04/26 17:33:39 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	free_if_builtin(t_pid_list **tmp, t_pid_list **next)
{
	if ((*tmp)->is_builtin)
	{
		*next = (*tmp)->next;
		free(*tmp);
		*tmp = *next;
		return (1);
	}
	return (0);
}

/*
	@brief		waits for a process to finish
	@param	tmp			pointer to the pid_list node
	@param	exit_status	pointer to the exit status
	@return	0
*/
static int	wait_for_process(t_pid_list *tmp, int *exit_status)
{
	int	status;

	waitpid(tmp->pid, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_status = 128 + WTERMSIG(status);
	return (0);
}

/*
	@brief		resolves the pid list by waiting for all nodes to finish
	@param	sh				pointer to the data struct
	@param	pid_list		pointer to the pid list
	@return	NULL

*/
void	*resolve_pid_list(t_data *sh, t_pid_list **pid_list)
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
		if (free_if_builtin(&tmp, &next))
			continue ;
		status = wait_for_process(tmp, &exit_status);
		if (status != 0)
			exit_status = status;
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	if (sh->exit_status == 0)
		sh->exit_status = exit_status;
	g_sigint_received = 0;
	return (NULL);
}

/*
	@brief		adds a pid to the pid list
	@param	pid			pid to add
	@param	pidlist		pointer to the pid list
	@param	is_builtin	whether the pid is a builtin
	@return	0			success
			1			malloc failed
*/
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
