/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/27 10:33:04 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// 0 -> nothing; 1 -> continue
static int	handle_sigint(t_data *sh, char **input)
{
	if (g_sigint_received == SIGINT)
	{
		g_sigint_received = 0;
		sh->exit_status = 1;
		if (*input != NULL)
			free(*input);
		return (1);
	}
	return (0);
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
	if (execute_executor(tree, sh) == -1)
		return (-1);
	return (0);
}

/// @brief Main loop of the program
void	run_input_loop(t_data *sh)
{
	char	*input;
	int		ret_val;

	while (sh->exit_flag == 0)
	{
		input = get_input(sh);
		if (input == NULL)
			break ;
		if (handle_sigint(sh, &input) == 1)
			continue ;
		ret_val = handle_empty_input(sh, &input);
		if (ret_val == 1)
			continue ;
		else if (ret_val == 2)
			break ;
		if (ret_val == 0)
		{
			gc_append_element(sh->gc, input);
			add_history(input);
		}
		execute_input(sh, input);
		gc_clean_and_reinit(&sh->gc);
	}
	gc_cleanup(sh->gc);
	clear_history();
}
