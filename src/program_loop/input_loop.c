/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/18 08:59:59 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint_received;

static void	print_heading_line(char	*line, int linenbr)
{
	int		i;

	i = -1;
	while (line[++i] != '\0')
	{
		if (linenbr < 6)
		{
			if (line[i] == '(' || line[i] == ')')
				printf("%s%c%s", ANSI_COLOR_RED, line[i], ANSI_COLOR_RESET);
			else if (line[i] == '/' || line[i] == '\\')
				printf("%s%c%s", ANSI_COLOR_YELLOW, line[i], ANSI_COLOR_RESET);
			else if (line[i] == '|' || line[i] == '_' || line[i] == '\'' || line[i] == '`' || line[i] == '-' || line[i] == ',' || line[i] == '<')
				printf("%s%c%s", ANSI_COLOR_CYAN, line[i], ANSI_COLOR_RESET);
			else
				printf("%c", line[i]);
		}
		else
		{
			if (line[i] == '(' || line[i] == ')')
				printf("%s%c%s", ANSI_COLOR_RED, line[i], ANSI_COLOR_RESET);
			else if (line[i] == '|' || line[i] == '/' || line[i] == '\\' || line[i] == '_' || line[i] == '\'' || line[i] == '`' || line[i] == '-' || line[i] == ',' || line[i] == '<')
				printf("%s%c%s", ANSI_COLOR_CYAN, line[i], ANSI_COLOR_RESET);
			else
				printf("%c", line[i]);
		
		}
	}
}

#define INPUT_FILE_LINES 10
#define INPUT_FILE_LINE_LENGTH 47

static void	*print_heading(void)
{
	int		fd;
	int		counter;
	char	*buffer;

	fd = open("./src/logo.txt", O_RDONLY);
	if (fd < 0)
		return (NULL);
	buffer = malloc(INPUT_FILE_LINE_LENGTH + 1);
	if (buffer == NULL)
		return (close(fd), NULL);
	buffer[INPUT_FILE_LINE_LENGTH] = '\0';
	counter = 0;
	while (counter < INPUT_FILE_LINES)
	{
		if (read(fd, buffer, INPUT_FILE_LINE_LENGTH) < 0)
			return (free(buffer), close(fd), NULL);
		print_heading_line(buffer, counter);
		counter++;
	}
	close (fd);
	free (buffer);
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

int	execute_input(t_program_data *program_data, char *input)
{
	t_list				*tokenified_input;
	t_list				*tmp_lst;
	t_bin_tree_node		*tree;
	t_token				*tmp;

	if (VERBOSE == 1)
		ft_printf("Received input: %s\n", input);
	// --- lexer
	tokenified_input = lexer(input, program_data);
	if (tokenified_input == NULL)
		return (-1); // handle error
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

	// printf("node: %p\nparent: %p\nvalue: %s\n", tree, tree->parent, tree->val[0]->value);
	if (VERBOSE == 1)
		print_binary_tree(tree, 0);
	if (VERBOSE == 1)
		ft_printf("\n\n\n");
	// --- executer
	pid_t last_pid = execute(tree, program_data);
	ft_printf("process returned with pid %d\n", last_pid);
	if (last_pid != -1)
		waitpid(last_pid, &program_data->exit_status, 0);
	ft_printf("done executing. now reprinting input prompt\n");
	return (0);
}

int	run_crash_interface(t_program_data *program_data)
{
	char	*input;

	print_logo();
	while (program_data->exit_flag == 0)
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
