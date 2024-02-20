#include "../include/minishell.h"

int run_crash_interface()
{
	printf("\n%s\t ██████╗██████╗  █████╗ ███████╗██╗  ██╗\n\t%s██╔════╝██╔══██╗██╔══██╗██╔════╝██║  ██║\n\t%s██║     ██████╔╝███████║███████╗███████║\n\t%s██║     ██╔══██╗██╔══██║╚════██║██╔══██║\n\t%s╚██████╗██║  ██║██║  ██║███████║██║  ██║\n\t%s ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝%s\n\n", ANSI_COLOR_RED, ANSI_COLOR_GREEN, ANSI_COLOR_YELLOW, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN, ANSI_COLOR_BLACK, ANSI_COLOR_RESET);
	ft_printf("Welcome to %sCRASH%s - crazy robust & advanced shell!\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);

	// get input
	// for now an always true condition works, later break out using exit either with a flag or by just ending program
	while (42 == 42)
	{
		char	*input;
		ft_printf("%scrash%s 💥 %s", ANSI_COLOR_RED, ANSI_COLOR_RESET, ANSI_COLOR_YELLOW);
		input = get_next_line(STDIN_FILENO);
		// remove trailing newline
		input[ft_strlen(input) - 1] = ' ';
		ft_printf("%s", ANSI_COLOR_RESET);
		test_lexer(input);
		free(input);
	}

	return (0);
}
