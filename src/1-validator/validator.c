/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:25:11 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/24 17:21:44 by niklasburch      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	throw_syntax_error(char *token)
{
	int	err;

	err = STDERR_FILENO;
	ft_putstr_fd("minishell: syntax error near unexpected token `", err);
	ft_putstr_fd(token, err);
	ft_putstr_fd("'\n", err);
}

int	check_files_and_clear(t_list **files)
{
	if (check_files(files[0], 0) != 0 || check_files(files[1], 1) != 0 || \
		check_files(files[2], 2) != 0)
	{
		ft_lstclear(&files[0], NULL);
		ft_lstclear(&files[1], NULL);
		ft_lstclear(&files[2], NULL);
		return (3);
	}
	ft_lstclear(&files[0], NULL);
	ft_lstclear(&files[1], NULL);
	ft_lstclear(&files[2], NULL);
	return (0);
}

int	process_tok_list(t_list **tok, int *brace_opened)
{
	t_tok	*token;
	int		error;

	token = (*tok)->content;
	error = check_token_errors(*tok, token);
	if (error != 0)
		return (error);
	error = check_brace_errors(*tok, token, brace_opened);
	if (error != 0)
		return (error);
	*tok = (*tok)->next;
	return (0);
}

void	null_fd_list(t_list *fd_list[3])
{
	fd_list[0] = NULL;
	fd_list[1] = NULL;
	fd_list[2] = NULL;
}

int	validate(t_list *tok_list)
{
	int		brace_opened;
	t_list	*files[3];
	t_tok	*tok;
	int		error;

	null_fd_list(files);
	brace_opened = 0;
	error = check_first_token(tok_list);
	if (error != 0)
		return (error);
	while (tok_list != NULL)
	{
		tok = tok_list->content;
		error = process_tok_list(&tok_list, &brace_opened);
		if (error != 0)
			return (error);
	}
	error = check_last_token(tok);
	if (error != 0)
		return (error);
	error = check_braces(brace_opened);
	if (error != 0)
		return (error);
	return (check_files_and_clear(files));
}
