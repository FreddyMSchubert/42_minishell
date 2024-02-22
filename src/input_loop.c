/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 13:11:16 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	*print_heading(void)
{
	int		fd;
	char	*line;
	int		color;

	fd = open("./src/logo.txt", O_RDONLY);
	if (fd < 0)
		return (NULL);
	color = 31;
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("\x1b[%dm%s%s", color, line, ANSI_COLOR_RESET);
		free(line);
		line = get_next_line(fd);
		if (color >= 36)
			color = 31;
		else
			color++;
	}
	free (line);
	return (NULL);
}

static void	*print_logo(void)
{
	printf("\033[H\033[J");
	print_heading();
	printf("   Welcome to %sCRASH%s, %s! - crazy robust & advanced shell!\n", \
							ANSI_COLOR_RED, ANSI_COLOR_RESET, getenv("USER"));
	printf("             %sMinishell created by ", ANSI_COLOR_CYAN);
	printf("\033]8;;https://profile.intra.42.fr/users/fschuber\033\\fschuber");
	printf("\033]8;;\033\\ & ");
	printf("\033]8;;https://profile.intra.42.fr/users/nburchha\033\\nburchha");
	printf("\033]8;;\033\\%s\n", ANSI_COLOR_RESET);
	printf("\n                               ---\n\n");
	return (NULL);
}

static int	execute_input(t_program_data *program_data, char *input)
{
	t_token				**tokenified_input;
	int					valid;
	t_bin_tree_node		*tree;

	// --- lexer
	tokenified_input = lexer(input, program_data);
	expander(tokenified_input, program_data);
	// --- validator
	valid = validator(tokenified_input);
	if (valid != 0)
	{
		cleanup(program_data->gc);
		program_data->gc = create_garbage_collector();
		return (-1);
	}
	// --- expander
	// expand_tokens(tokenified_input);
	// --- parser
	tree = tok_to_bin_tree(tokenified_input);
	// --- executer
	execute(tree, program_data);
	return (0);
}

int	run_crash_interface(t_program_data *program_data)
{
	char	*input;

	print_logo();
	while (program_data->exit_flag == 0)
	{
		ft_printf("%scrash%s 💥 ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		input = get_next_line(STDIN_FILENO);
		append_element(program_data->gc, input);
		if (input[0] == '\n')
		{
			cleanup(program_data->gc);
			program_data->gc = create_garbage_collector();
			continue ;
		}
		input[ft_strlen(input) - 1] = '\0';
		if (execute_input(program_data, input) == -1)
			continue ;
		cleanup(program_data->gc);
		program_data->gc = create_garbage_collector();
	}
	cleanup(program_data->gc);
	return (0);
}
