/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:34:26 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_input(t_data *sh, char *input)
{
	t_list				*tokenified_input;
	t_node				*tree;
	int					valid;

	if (VERBOSE == 1)
		printf("Received input: %s\n", input);
	input = expand(input, sh, false);
	if (VERBOSE == 1)
		printf("after expanding:\ninput:\n");
	// --- lex
	tokenified_input = lexer(input, sh);
	if (tokenified_input == NULL)
		return (-1);
	if (VERBOSE == 1)
		print_tokens(tokenified_input);
	// --- validate
	valid = validate(tokenified_input);
	sh->exit_status = valid;
	if (valid != 0)
	{
		if (VERBOSE == 1)
			printf("token sequence is invalid: %d\n", valid);
		if (!isatty(fileno(stdin)))
			sh->exit_flag = 1;
		gc_cleanup(sh->gc);
		sh->gc = gc_create();
		return (-1);
	}
	if (VERBOSE == 1)
		printf("token sequence is valid\n");
	tokenified_input = switch_redir_args(tokenified_input);
	tree = parse(tokenified_input, sh);
	tree->parent = NULL;

	if (VERBOSE == 1)
        print_node_with_children(tree, 0);
	if (VERBOSE == 1)
		printf("\n\n\n");
	// --- executer
	sh->pid_list = NULL;
	execute(tree, sh, &sh->pid_list);
	resolve_pid_list(sh, &sh->pid_list);
	return (0);
}

int	run_input_loop(t_data *sh)
{
	char	*input;
	char	*line;

	if (DEBUG == 0)
		print_logo();
	while (sh->exit_flag == 0)
	{
		if (isatty(fileno(stdin)))
		{
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
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (line == NULL)
				return (gc_cleanup(sh->gc), 0);
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (g_sigint_received == SIGINT)
		{
			g_sigint_received = 0;
			sh->exit_status = 1;
			if (input != NULL)
				free(input);
			continue ;
		}
		if (input == NULL || ft_isspace_str_all(input) == 1)
		{
			if (input != NULL)
				gc_append_element(sh->gc, input);
			gc_cleanup(sh->gc);
			sh->gc = gc_create();
			sh->exit_status = 0;
			if (input == NULL)
				break ;
			continue ;
		}
		else
		{
			gc_append_element(sh->gc, input);
			add_history(input);
		}
		execute_input(sh, input);
		gc_cleanup(sh->gc);
		sh->gc = gc_create();
	}
	gc_cleanup(sh->gc);
	clear_history();
	return (0);
}
