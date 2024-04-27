/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:31:50 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/27 10:32:06 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_input_from_terminal(t_data *sh)
{
	char	*input;

	if (sh->exit_status == 0)
	{
		printf("%s", ANSI_COLOR_CYAN);
		input = readline("crash > ");
	}
	else
	{
		printf("%s", ANSI_COLOR_RED);
		input = readline("crash > ");
	}
	printf("%s", ANSI_COLOR_RESET);
	return (input);
}

static char	*get_input_from_file(t_data *sh)
{
	char	*input;
	char	*line;

	line = get_next_line(fileno(stdin));
	if (line == NULL)
		return (gc_cleanup(sh->gc), NULL);
	input = ft_strtrim(line, "\n");
	free(line);
	return (input);
}

char	*get_input(t_data *sh)
{
	char	*input;

	if (isatty(STDIN_FILENO))
		input = get_input_from_terminal(sh);
	else
		input = get_input_from_file(sh);
	return (input);
}

// 0 -> nothing; 1 -> continue; 2 -> break
void	handle_empty_input_append_to_gc(t_data *sh, char **input)
{
	if (*input == NULL || ft_isspace_str_all(*input) == 1)
	{
		gc_cleanup(sh->gc);
		sh->gc = gc_create();
		if (*input == NULL)
			return (2);
		return (1);
	}
	return (0);
}
