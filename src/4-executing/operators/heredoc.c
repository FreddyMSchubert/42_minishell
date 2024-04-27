/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 07:54:00 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/27 09:56:43 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	setup_heredoc(t_node *node, t_data *data, int *pipe_fd)
{
	if (node->r->val[0]->type == TOK_REDIR)
		redirect(node->r, data);
	if (pipe(pipe_fd) < 0)
		return (-1);
	return (0);
}

static int	finish_heredoc(int *pipe_fd, t_node *node)
{
	if (g_sigint_received == SIGINT)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		g_sigint_received = 0;
		return (-2);
	}
	close(pipe_fd[1]);
	node->l->in_fd = pipe_fd[0];
	return (0);
}

static char	*get_line(t_data *data)
{
	char	*line;

	if (isatty(fileno(stdin)))
		line = readline("crash_doc 📄 ");
	else
	{
		write(0, "crash_doc 📄 ", 12);
		line = get_next_line(fileno(stdin));
		if (line == NULL)
			return (NULL);
		gc_append_element(data->gc, line);
		line[ft_strlen(line) - 1] = '\0';
	}
	return (line);
}

static char	*convert_line(char *line, t_data *data, t_node *node)
{
	char	*converted_line;

	if (ft_strncmp(line, "", 1) == 0)
		return (line);
	if (node->r->val[0]->type != TOK_D_QUOTE \
		&& node->r->val[0]->type != TOK_S_QUOTE)
		converted_line = expand(line, data, true);
	else
	{
		converted_line = ft_strdup(line);
		gc_append_element(data->gc, converted_line);
	}
	return (converted_line);
}

int	heredoc(t_node *node, t_data *data)
{
	int		pipe_fd[2];
	char	*line;
	char	*converted_line;

	if (setup_heredoc(node, data, pipe_fd) != 0)
		return (-1);
	while (g_sigint_received != SIGINT)
	{
		line = get_line(data);
		if (!line || (ft_strncmp(line, get_filename(node), \
			ft_strlen(line)) == 0 && ft_strlen(line) > 0) \
			|| g_sigint_received == SIGINT)
			break ;
		converted_line = convert_line(line, data, node);
		if (!converted_line)
			return (log_err("error expanding heredoc", "redir", NULL), \
						finish_heredoc(pipe_fd, node), -1);
		write(pipe_fd[1], converted_line, ft_strlen(converted_line));
		write(pipe_fd[1], "\n", 1);
	}
	return (finish_heredoc(pipe_fd, node));
}
