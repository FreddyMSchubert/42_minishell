/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/04 13:31:02 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint_received;

int	execute_input(t_program_data *program_data, char *input)
{
	t_list				*tokenified_input;
	t_list				*tmp_lst;
	t_bin_tree_node		*tree;
	t_token				*tmp;
	pid_t				last_pid;

	if (VERBOSE == 1)
		ft_printf("Received input: %s\n", input);
	// --- lexer
	tokenified_input = lexer(input, program_data);
	if (tokenified_input == NULL)
		return (ft_putstr_fd("crash: error lexing input", STDERR_FILENO), -1);
	if (VERBOSE == 1)
		print_tokens(tokenified_input);
	tmp_lst = tokenified_input;
	while (tmp_lst)
	{
		tmp = (t_token *)tmp_lst->content;
		tmp->value = get_rid_of_quotes(tmp->value, program_data);
		tmp_lst = tmp_lst->next;
	}
	if (VERBOSE == 1)
		ft_printf("token sequence is valid\n");
	expander(tokenified_input, program_data);
	if (VERBOSE == 1)
	{
		ft_printf("after expanding:\n");
		print_tokens(tokenified_input);
	}
	// --- validator
	// valid = validator(tokenified_input);
	// if (valid != 0)
	// {
	// 	if (VERBOSE == 1)
	// 		ft_printf("token sequence is invalid: %d\n", valid);
	// 	gc_cleanup(program_data->gc);
	// 	program_data->gc = create_garbage_collector();
	// 	return (-1);
	// }
	if (VERBOSE == 1)
		ft_printf("token sequence is valid\n");
	// tokenified_input = switch_args_for_redir(tokenified_input);
	tree = tok_to_bin_tree(tokenified_input, program_data);
	tree->parent = NULL;

	if (VERBOSE == 1)
		print_binary_tree(tree, 0);
	if (VERBOSE == 1)
		ft_printf("\n\n\n");
	// --- executer
	last_pid = execute(tree, program_data);
	if (last_pid != -1)
		waitpid(last_pid, &program_data->exit_status, 0);
	return (0);
}

int	run_crash_interface(t_program_data *program_data)
{
	char	*input;

	if (DEBUG == 0)
		print_logo();
	while (program_data->exit_flag == 0)
	{
		if (DEBUG == 0)
		{
			if (program_data->exit_status == 0)
			{
				ft_printf("%s", ANSI_COLOR_CYAN);
				input = readline("crash 💣 ");
			}
			else
			{
				ft_printf("%s", ANSI_COLOR_RED);
				input = readline("crash 💥 ");
			}
			ft_printf("%s", ANSI_COLOR_RESET);
		}
		else
			if (isatty(fileno(stdin)))
				input = readline("bash-3.2$ ");
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
			program_data->gc = create_garbage_collector();
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
		program_data->gc = create_garbage_collector();
	}
	gc_cleanup(program_data->gc);
	rl_clear_history();
	return (0);
}
