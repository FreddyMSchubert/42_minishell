/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:31:50 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/26 17:32:22 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_input_from_terminal(t_data *sh)
{
	char	*input;

	if (sh->exit_status == 0)
	{
		printf("%s", ANSI_COLOR_CYAN);
		input = readline("crash 💣 ");
	}
	else
	{
		printf("%s", ANSI_COLOR_RED);
		input = readline("crash 💥 ");
	}
	printf("%s", ANSI_COLOR_RESET);
	return (input);
}

char	*get_input_from_file(void)
{
	char	*input;
	char	*line;

	line = get_next_line(fileno(stdin));
	if (line == NULL)
		return (NULL);
	input = ft_strtrim(line, "\n");
	free(line);
	return (input);
}

void	handle_empty_input(t_data *sh, char **input)
{
	if (*input == NULL || ft_isspace_str_all(*input) == 1)
	{
		if (*input != NULL)
			gc_append_element(sh->gc, *input);
		gc_cleanup(sh->gc);
		sh->gc = gc_create();
		sh->exit_status = 0;
	}
}
