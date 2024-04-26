/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 16:16:23 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_input(t_data *program_data, char *input)
{
	t_list				*tokenified_input;
	t_node				*tree;
	int					valid;

	if (VERBOSE == 1)
		printf("Received input: %s\n", input);
	input = expand(input, program_data, false);
	if (VERBOSE == 1)
		printf("after expanding:\ninput:\n");
	// --- lex
	tokenified_input = lexer(input, program_data);
	if (tokenified_input == NULL)
		return (-1);
	if (VERBOSE == 1)
		print_tokens(tokenified_input);
	// --- validate
	valid = validate(tokenified_input);
	program_data->exit_status = valid;
	if (valid != 0)
	{
		if (VERBOSE == 1)
			printf("token sequence is invalid: %d\n", valid);
		if (!isatty(fileno(stdin)))
			program_data->exit_flag = 1;
		gc_cleanup(program_data->gc);
		program_data->gc = gc_create();
		return (-1);
	}
	if (VERBOSE == 1)
		printf("token sequence is valid\n");
	tokenified_input = switch_redir_args(tokenified_input);
	tree = parse(tokenified_input, program_data);
	tree->parent = NULL;

	if (VERBOSE == 1)
        print_node_with_children(tree, 0);
	if (VERBOSE == 1)
		printf("\n\n\n");
	// --- executer
	program_data->pid_list = NULL;
	execute(tree, program_data, &program_data->pid_list);
	resolve_pid_list(program_data, &program_data->pid_list);
	return (0);
}

int	run_input_loop(t_data *program_data)
{
	char	*input;
	char	*line;

	if (DEBUG == 0)
		print_logo();
	while (program_data->exit_flag == 0)
	{
		if (isatty(fileno(stdin)))
		{
			if (program_data->exit_status == 0)
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
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (line == NULL)
				return (gc_cleanup(program_data->gc), 0);
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (g_sigint_received == SIGINT)
		{
			g_sigint_received = 0;
			program_data->exit_status = 1;
			if (input != NULL)
				free(input);
			continue ;
		}
		if (input == NULL || ft_isspace_str_all(input) == 1)
		{
			if (input != NULL)
				gc_append_element(program_data->gc, input);
			gc_cleanup(program_data->gc);
			program_data->gc = gc_create();
			program_data->exit_status = 0;
			if (input == NULL)
				break ;
			continue ;
		}
		else
		{
			gc_append_element(program_data->gc, input);
			add_history(input);
		}
		execute_input(program_data, input);
		gc_cleanup(program_data->gc);
		program_data->gc = gc_create();
	}
	gc_cleanup(program_data->gc);
	clear_history();
	return (0);
}
