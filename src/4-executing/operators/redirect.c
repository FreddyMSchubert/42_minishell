/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 07:58:13 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	redirect(t_node *node, t_data *program_data)
{
	int		fd;
	int		flags;
	char	*filename;
	bool	redir_out;

	node->redirected = true;
	if (node->in_fd != 0 && node->val[0]->val[0] == '>' && node->l)
		node->l->in_fd = node->in_fd;
	else if (node->in_fd != 0)
		close(node->in_fd);
	if (node->out_fd != 1 && node->val[0]->val[0] == '<' && node->l)
		node->l->out_fd = node->out_fd;
	else if (node->out_fd != 1)
		close(node->out_fd);
	if (ft_strncmp(node->val[0]->val, "<<", 2) == 0)
		return (heredoc(node, program_data));
	if (ft_strncmp(node->val[0]->val, "<", 1) == 0)
		flags = O_RDONLY;
	else if (ft_strncmp(node->val[0]->val, ">>", 2) == 0)
		flags = O_APPEND | O_WRONLY | O_CREAT;
	else
		flags = O_TRUNC | O_WRONLY | O_CREAT;
	filename = get_filename(node);
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		if (node->in_fd != 0)
			close(node->in_fd);
		if (node->out_fd != 1)
			close(node->out_fd);
		program_data->exit_status = 1;
		return (1);
	}
	if (node->r->val[0]->type == TOK_REDIR && ft_strncmp(node->val[0]->val, node->r->val[0]->val, 1) == 0)
		return (close(fd), redirect(node->r, program_data));
	if (node->val[0]->val[0] == '>')
		redir_out = true;
	else
		redir_out = false;
	while (node->parent && node->parent->val[0]->type == TOK_REDIR)
	{
		node = node->parent;
		if (!node->parent || (node->parent->val && node->parent->val[0]->type != TOK_REDIR))
		{
			if (!redir_out && node->l)
				node->l->in_fd = fd;
			else if (node->l)
				node->l->out_fd = fd;
			return (0);
		}
	}
	if (node->val[0]->val[0] == '<' && node->l)
		node->l->in_fd = fd;
	else if (node->l)
		node->l->out_fd = fd;
	(void)program_data;
	return (0);
}
