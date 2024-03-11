/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/11 12:29:39 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char *get_filename(t_bin_tree_node *node)
{
	t_bin_tree_node *temp;

	temp = node->r;
	while (temp->val[0]->type != TOK_CMD_ARG)
		temp = temp->l;
	return (temp->val[0]->value);
}

int	heredoc_to_pipe(char *delimiter, int pipe_fd[2])
{
    char	*line;

    line = NULL;
    if (pipe(pipe_fd) < 0)
        return (perror("pipe creation failed"), -1);        // actual error message?
    while (1)
    {
        line = readline("crash_doc 📄 ");
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    return (0);
}

int	heredoc(t_bin_tree_node *node)
{
    int         pipe_fd[2];
    char        *line;
    char        *delimiter;

    delimiter = node->r->val[0]->value;
    if (pipe(pipe_fd) < 0)
        return (perror("pipe creation failed"), -1);        // actual error message?
    while (1)
    {
        line = readline("crash_doc 📄 ");
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    node->l->input_fd = pipe_fd[0];
	return (0);
}

int	redirect(t_bin_tree_node *node, t_program_data *program_data)
{
	int		fd;
	int		flags;
	char	*filename;
	bool	redir_out;

	filename = get_filename(node);
	if (node->input_fd != 0)
		node->l->input_fd = node->input_fd;
	if (node->output_fd != 1)
		node->l->output_fd = node->output_fd;
	if (ft_strncmp(node->val[0]->value, "<<", 2) == 0)
		return (heredoc(node));
	// printf("node->val[0]->value: %s\n", node->val[1]->value);
	if (ft_strncmp(node->val[0]->value, "<", 1) == 0)
		flags = O_RDONLY;
	else if (ft_strncmp(node->val[0]->value, ">>", 2) == 0)
		flags = O_APPEND | O_WRONLY | O_CREAT;
	else
		flags = O_TRUNC | O_WRONLY | O_CREAT;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		ft_printf("minishell: %s: %s\n", node->r->val[0]->value, strerror(errno));
		return (-1);
	}
	if (node->r->val[0]->type == TOK_REDIR && ft_strncmp(node->val[0]->value, node->r->val[0]->value, 1) == 0) // if its not the last redirect and the same as the current one
		return (printf("another redirection in front, node: %s\n", node->val[0]->value), close(fd), redirect(node->r, program_data));
	if (node->val[0]->value[0] == '>')
		redir_out = true;
	else
		redir_out = false;
	while (node->parent && node->parent->val[0]->type == TOK_REDIR)
	{
		node = node->parent;
		if (!node->parent || (node->parent->val && node->parent->val[0]->type != TOK_REDIR))
		{
			if (!redir_out)
				node->l->input_fd = fd;
			else
				node->l->output_fd = fd;
			return (0);
		}
	}
	if (ft_strncmp(node->val[0]->value, "<", 1) == 0)
		node->l->input_fd = fd;
	else
		node->l->output_fd = fd;
	// printf("end of redir, node: %s, output_fd: %d\n", node->l->val[0]->value, fd);
	(void)program_data;
	return (0);
}
