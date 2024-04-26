/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 22:16:59 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_sigint(t_data *sh, char **input)
{
	if (g_sigint_received == SIGINT)
	{
		g_sigint_received = 0;
		sh->exit_status = 1;
		if (*input != NULL)
			free(*input);
	}
}

static int	execute_input(t_data *sh, char *input)
{
	t_list	*tokenified_input;
	t_node	*tree;

	if (VERBOSE == 1)
		printf("Received input: %s\n", input);
	if (execute_expander(&input, sh) == -1)
		return (-1);
	if (execute_lexer(input, sh, &tokenified_input) == -1)
		return (-1);
	if (execute_validator(tokenified_input, sh) == -1)
		return (-1);
	tokenified_input = switch_redir_args(tokenified_input);
	if (execute_parser(tokenified_input, sh, &tree) == -1)
		return (-1);
	if (VERBOSE == 1)
		printf("\n\n\n");
	if (execute_executor(tree, sh) == -1)
		return (-1);
	return (0);
}

int	run_input_loop(t_data *sh)
{
	char	*input;

	if (DEBUG == 0)
		print_logo();
	while (sh->exit_flag == 0)
	{
		if (isatty(fileno(stdin)))
			input = get_input_from_terminal(sh);
		else
			input = get_input_from_file();
		if (input == NULL)
			return (gc_cleanup(sh->gc), 0);
		handle_sigint(sh, &input);
		handle_empty_input(sh, &input);
		if (input == NULL)
			break ;
		execute_input(sh, input);
		gc_cleanup(sh->gc);
		sh->gc = gc_create();
	}
	gc_cleanup(sh->gc);
	clear_history();
	return (0);
}
