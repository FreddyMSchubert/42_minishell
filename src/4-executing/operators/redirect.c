/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 06:30:42 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

extern int	g_sigint_received;

char	*get_filename(t_node *node)
{
	t_node	*temp;

	temp = node->r;
	while (temp->val[0]->type > TOK_D_QUOTE)
		temp = temp->l;
	return (temp->val[0]->value);
}

static int	heredoc(t_node *node, t_data	*program_data)
{
	int		pipe_fd[2];
	char	*line;
	char	*converted_line;
	char	*delimiter;
	bool	shouldexpand;

	if (node->r->val[0]->type == TOK_REDIR)
		redirect(node->r, program_data);
	shouldexpand = true;
	delimiter = get_filename(node);
	if (VERBOSE)
		printf("delimiter: %s\n", delimiter);
	if (node->r->val[0]->type == TOK_D_QUOTE || node->r->val[0]->type == TOK_S_QUOTE)
		shouldexpand = false;
	if (pipe(pipe_fd) < 0)
		return (-1);
	while (g_sigint_received != SIGINT)
	{
		ft_putstr_fd("crash_doc 📄 ", STDOUT_FILENO);
		line = get_next_line(fileno(stdin));
		if (line == NULL)
			return (0);
		gc_append_element(program_data->gc, line);
		line[ft_strlen(line) - 1] = '\0';
		if (!line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0 || \
			g_sigint_received == SIGINT)
			break ;
		if (shouldexpand)
			converted_line = expand(line, program_data, true);
		else
		{
			converted_line = ft_strdup(line);
			gc_append_element(program_data->gc, converted_line);
		}
		if (!converted_line)
			return (ft_putstr_fd("crash: redir: error expanding heredoc", STDERR_FILENO), -1);
		write(pipe_fd[1], converted_line, ft_strlen(converted_line));
		write(pipe_fd[1], "\n", 1);
	}
	if (g_sigint_received == SIGINT)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		g_sigint_received = 0;
		return (-2);
	}
	close(pipe_fd[1]);
	node->l->input_fd = pipe_fd[0];
	return (0);
}

int	redirect(t_node *node, t_data *program_data)
{
	int		fd;
	int		flags;
	char	*filename;
	bool	redir_out;

	node->redirected = true;
	if (node->input_fd != 0 && node->val[0]->value[0] == '>' && node->l)
		node->l->input_fd = node->input_fd;
	else if (node->input_fd != 0)
		close(node->input_fd);
	if (node->output_fd != 1 && node->val[0]->value[0] == '<' && node->l)
		node->l->output_fd = node->output_fd;
	else if (node->output_fd != 1)
		close(node->output_fd);
	if (ft_strncmp(node->val[0]->value, "<<", 2) == 0)
		return (heredoc(node, program_data));
	if (ft_strncmp(node->val[0]->value, "<", 1) == 0)
		flags = O_RDONLY;
	else if (ft_strncmp(node->val[0]->value, ">>", 2) == 0)
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
		if (node->input_fd != 0)
			close(node->input_fd);
		if (node->output_fd != 1)
			close(node->output_fd);
		program_data->exit_status = 1;
		return (1);
	}
	if (node->r->val[0]->type == TOK_REDIR && ft_strncmp(node->val[0]->value, node->r->val[0]->value, 1) == 0) // if its not the last redirect and the same as the current one
		return (close(fd), redirect(node->r, program_data)); //printf("another redirection in front, node: %s\n", node->val[0]->value), 
	if (node->val[0]->value[0] == '>')
		redir_out = true;
	else
		redir_out = false;
	while (node->parent && node->parent->val[0]->type == TOK_REDIR)
	{
		node = node->parent;
		if (!node->parent || (node->parent->val && node->parent->val[0]->type != TOK_REDIR))
		{
			if (!redir_out && node->l)
				node->l->input_fd = fd;
			else if (node->l)
				node->l->output_fd = fd;
			return (0);
		}
	}
	if (node->val[0]->value[0] == '<' && node->l)
		node->l->input_fd = fd;
	else if (node->l)
		node->l->output_fd = fd;
	(void)program_data;
	return (0);
}
