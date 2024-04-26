/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 09:16:40 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	open_file(t_node *node, t_data *data)
{
	int		fd;
	int		flags;
	char	*filename;

	if (ft_strncmp(node->val[0]->val, "<", 1) == 0)
		flags = O_RDONLY;
	else if (ft_strncmp(node->val[0]->val, ">>", 2) == 0)
		flags = O_APPEND | O_WRONLY | O_CREAT;
	else
		flags = O_TRUNC | O_WRONLY | O_CREAT;
	filename = get_filename(node);
	fd = open(filename, flags, 0644);
	if (fd < 0)
		return (log_err(strerror(errno), filename, NULL), \
						close_fds(node), data->exit_status = 1, 1);
	return (fd);
}

static void	propagate_descriptor(t_node *node, int fd)
{
	bool	redir_out;

	if (node->val[0]->val[0] == '>')
		redir_out = true;
	else
		redir_out = false;
	while (node->parent && node->parent->val[0]->type == TOK_REDIR)
	{
		node = node->parent;
		if (!node->parent || (node->parent->val && \
								node->parent->val[0]->type != TOK_REDIR))
		{
			if (!redir_out && node->l)
				node->l->in_fd = fd;
			else if (node->l)
				node->l->out_fd = fd;
			return ;
		}
	}
	if (node->val[0]->val[0] == '<' && node->l)
		node->l->in_fd = fd;
	else if (node->l)
		node->l->out_fd = fd;
}

int	redirect(t_node *node, t_data *program_data)
{
	int		fd;

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
	fd = open_file(node, program_data);
	if (fd < 0)
		return (fd);
	if (node->r->val[0]->type == TOK_REDIR && \
				ft_strncmp(node->val[0]->val, node->r->val[0]->val, 1) == 0)
		return (close(fd), redirect(node->r, program_data));
	propagate_descriptor(node, fd);
	return (0);
}
