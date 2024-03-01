/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 15:25:11 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/01 16:22:25 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
5		// Redirections (<, >, >>, <<)
6		// Pipe (|)
7		// Logical Operators (&&, ||)


error codes:
0 for no error
1 for consecutive operators
2 for unclosed braces
3 for file error
5 for allocation error
*/

int	add_file_to_list(t_list **files, char *value, int *error_code)
{
	t_list	*tmp;

	// printf("error_code: %s\n", value);
	tmp = ft_lstnew(value);
	if (!tmp)
		*error_code = 5;
	if (*error_code == 0)
		ft_lstadd_back(files, tmp);
	return (*error_code);
}

int	validator(t_token **token_arr)
{
	int	i;
	int	brace_opened;
	int	error_code;
	t_list	*files[3]; // 0 for input, 1 for output, 2 for append_out

	i = -1;
	files[0] = NULL;
	files[1] = NULL;
	files[2] = NULL;
	brace_opened = 0;
	error_code = 0;
	while (token_arr[++i] != NULL)
	{
		// printf("token_arr: %s\n", token_arr[i]->value);
		// check for consecutive pipes and or logical operators
		if (token_arr[i]->type >= 6 && token_arr[i]->type <= 7)
			if (token_arr[i + 1] && token_arr[i + 1]->type >= 6 && token_arr[i + 1]->type <= 7)
				return (1);
		

		// check for unclosed braces
		if (token_arr[i]->type == TOK_OPEN_BRACE)
			brace_opened++;
		else if (token_arr[i]->type == TOK_CLOSE_BRACE)
			brace_opened--;

		// check for valid word after < > >> <<
		if (token_arr[i]->type == TOK_REDIR)
			if (!token_arr[i + 1] || token_arr[i + 1]->type > 2)
				return (3);
		// put in files into linked lists
		if (token_arr[i]->type == TOK_REDIR && ft_strncmp(token_arr[i]->value, "<", 2) == 0 && token_arr[i + 1])
			if (add_file_to_list(&files[0], token_arr[i + 1]->value, &error_code) != 0)
				return (error_code);
		// put append files into linked lists
		if (token_arr[i]->type == TOK_REDIR && ft_strncmp(token_arr[i]->value, ">>", 2) == 0 && token_arr[i + 1])
			if (add_file_to_list(&files[2], token_arr[i + 1]->value, &error_code) != 0)
				return (error_code);
		// put output files into linked lists
		if (token_arr[i]->type == TOK_REDIR && ft_strncmp(token_arr[i]->value, ">", 2) == 0 && token_arr[i + 1])
			if (add_file_to_list(&files[1], token_arr[i + 1]->value, &error_code) != 0)
				return (error_code);
	}
	// check for | in beginning or end
	if (token_arr[0]->type == 6 || token_arr[i - 1]->type == 6)
		return (1);
	if (brace_opened != 0)
		return (2);
	// check for file errors
	if (check_files(files[0], 0) != 0 || check_files(files[1], 1) != 0 || check_files(files[2], 2) != 0)
		return (ft_lstclear(&files[0], NULL), ft_lstclear(&files[1], NULL), ft_lstclear(&files[2], NULL), 3);
	return (ft_lstclear(&files[0], NULL), ft_lstclear(&files[1], NULL), ft_lstclear(&files[2], NULL), 0);
}
