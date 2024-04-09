/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/09 12:57:37 by nburchha         ###   ########.fr       */
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

// static char	*expand_heredoc_line(char	*line, t_program_data *program_data)
// {
// 	t_list	*list;
// 	t_token	*token;
// 	char	*expanded_line;

// 	list = malloc (sizeof(t_list));
// 	if (!list)
// 		return (NULL);
// 	token = malloc (sizeof(t_token));
// 	if (!token)
// 		return (NULL);
// 	token->value = line;
// 	token->type = TOK_WORD;
// 	list->content = token;
// 	list->next = NULL;
// 	expander(list, program_data);
// 	expanded_line = ft_strdup(((t_token *)list->content)->value);
// 	free (token);
// 	free (list);
// 	return (expanded_line);
// }

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
	while (g_sigint_received != SIGINT)
	{
		line = readline("crash_doc 📄 ");
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
		free(line);
		free(converted_line);
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

int	redirect(t_bin_tree_node *node, t_program_data *program_data)
{
	int		fd;
	int		flags;
	char	*filename;
	bool	redir_out;

	if (node->input_fd != 0)
		node->l->input_fd = node->input_fd;
	if (node->output_fd != 1)
		node->l->output_fd = node->output_fd;
	if (ft_strncmp(node->val[0]->value, "<<", 2) == 0)
		return (heredoc(node, program_data));
	// printf("node->val[0]->value: %s\n", node->val[1]->value);
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
		return (-2);
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
			if (!redir_out)
				node->l->input_fd = fd;
			else
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
