#include "../include/minishell.h"

int run_crash_interface(t_program_data *program_data)
{
	char	*input;

	printf("\033[H\033[J\n%s\t ██████╗██████╗  █████╗ ███████╗██╗  ██╗\n\t%s██╔════╝██╔══██╗██╔══██╗██╔════╝██║  ██║\n\t%s██║     ██████╔╝███████║███████╗███████║\n\t%s██║     ██╔══██╗██╔══██║╚════██║██╔══██║\n\t%s╚██████╗██║  ██║██║  ██║███████║██║  ██║\n\t%s ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝%s\n\n", ANSI_COLOR_RED, ANSI_COLOR_GREEN, ANSI_COLOR_YELLOW, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN, ANSI_COLOR_WHITE, ANSI_COLOR_RESET);
	ft_printf("Welcome to %sCRASH%s, %s! - crazy robust & advanced shell!\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, getenv("USER"));
	ft_printf("              %sCreated by \033]8;;https://profile.intra.42.fr/users/fschuber\033\\fschuber\033]8;;\033\\ & \033]8;;https://profile.intra.42.fr/users/nburchha\033\\nburchha\033]8;;\033\\%s\n\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);

	(void) program_data;

	// get input
	// for now an always true condition works, later break out using exit either with a flag or by just ending program
	while (program_data->exit_flag == 0)
	{
		ft_printf("%scrash%s 💥 ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		input = get_next_line(STDIN_FILENO);
		appendElement(program_data->gc, input);
		if (input[0] == '\n')
		{
			cleanup(program_data->gc);
			continue;
		}
		input[ft_strlen(input) - 1] = '\0';

		// lexer
		t_token **tokenified_input = lexer(input);
		// validator
		int valid = validator(tokenified_input);
		if (valid != 0)
		{
			broadcast_validator_error(valid);
			cleanup(program_data->gc);
			program_data->gc = createGarbageCollector();
			continue;
		}
		// expander
		// expand_tokens(tokenified_input);
		// parser
		t_bin_tree_node *tree = tok_to_bin_tree(tokenified_input);
		// executer
		execute(tree, program_data);

		// cleanup
		cleanup(program_data->gc);
		program_data->gc = createGarbageCollector();
	}
	cleanup(program_data->gc);
	for (int i = 0; program_data->envcp[i]; i++)
		free(program_data->envcp[i]);
	free(program_data->envcp);

	return (0);
}
