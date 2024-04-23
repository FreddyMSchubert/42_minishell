/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/23 10:43:20 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

extern int	g_sigint_received;

char	*get_filename(t_bin_tree_node *node)
{
	t_bin_tree_node	*temp;

	temp = node->r;
	while (temp->val[0]->type > TOK_D_QUOTE)
		temp = temp->l;
	return (temp->val[0]->value);
}

static int	heredoc(t_bin_tree_node *node, t_program_data	*program_data)
{
	int		pipe_fd[2];
	char	*line;
	char	*converted_line;
	char	*delimiter;
	bool	expand;

	expand = true;
	delimiter = node->r->val[0]->value;
	if (delimiter[0] == '"' || delimiter[0] == '\'')
	{
		delimiter++;
		delimiter[ft_strlen(delimiter) - 1] = '\0';
		expand = false;
	}
	if (pipe(pipe_fd) < 0)
		return (-1);
	fd_collector_append(program_data, pipe_fd[0], pipe_fd[1]);
	while (g_sigint_received != SIGINT)
	{
		if (isatty(fileno(stdin)))
			line = readline("crash_doc 📄 ");
		else
		{
			line = get_next_line(fileno(stdin));
			if (line == NULL)
				return (0);
			line[ft_strlen(line) - 1] = '\0';
		}
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 || \
			g_sigint_received == SIGINT)
		{
			free(line);
			break ;
		}
		if (expand)
			converted_line = expand_values(line, program_data, true);
		else
			converted_line = ft_strdup(line);
		if (!converted_line)
			return (ft_putstr_fd("crash: redir: error expanding heredoc", STDERR_FILENO), -1);
		write(pipe_fd[1], converted_line, ft_strlen(converted_line));
		write(pipe_fd[1], "\n", 1);
	}
	if (g_sigint_received == SIGINT)
		return (g_sigint_received = 0, -2);
	node->l->input_fd = pipe_fd[0];
	return (0);
}

int	redirect(t_bin_tree_node *node, t_program_data *program_data)
{
	int		fd;
	int		flags;
	char	*filename;
	bool	redir_out;

	if (node->input_fd != 0 && node->l)
		node->l->input_fd = node->input_fd;
	if (node->output_fd != 1 && node->l)
		node->l->output_fd = node->output_fd;
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
	fd_collector_append(program_data, fd, -1);
	if (fd < 0)
		return (log_err(filename, filename, NULL), program_data->exit_status = 1, 1);
	if (node->r->val[0]->type == TOK_REDIR && ft_strncmp(node->val[0]->value, node->r->val[0]->value, 1) == 0) // if its not the last redirect and the same as the current one
		return (redirect(node->r, program_data));
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

// recursive check upwards until reaching head node.
// if any parent node has a redirected output, return that instead.
// topmost redirected output should be returned in the end
// if mode is 0, it returns the lowest different output fd, if mode is 1, it returns the highest
int	get_parent_output_fd(t_bin_tree_node *node, char mode)
{
	int	topmost_parent_output_fd;
	int	own_output_fd;

	if (node->parent)
		topmost_parent_output_fd = get_parent_output_fd(node->parent, mode);
	else
		return (node->output_fd);
	own_output_fd = node->output_fd;
	if (mode == 0)
	{
		if (own_output_fd != STDOUT_FILENO)
			return (own_output_fd);
		return (topmost_parent_output_fd);
	}
	else
	{
		if (topmost_parent_output_fd != STDOUT_FILENO)
			return (topmost_parent_output_fd);
		return (own_output_fd);
	}
}
