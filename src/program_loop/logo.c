/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 08:50:24 by fschuber          #+#    #+#             */
/*   Updated: 2024/03/20 09:18:44 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_heading_line(char	*line, int linenbr)
{
	int		i;

	i = -1;
	while (line[++i] != '\0')
	{
		if (strchr("()", line[i]))
			printf("%s%c%s", ANSI_COLOR_RED, line[i], ANSI_COLOR_RESET);
		else if (linenbr < 6)
		{
			if (strchr("/\\", line[i]))
				printf("%s%c%s", ANSI_COLOR_YELLOW, line[i], ANSI_COLOR_RESET);
			else if (strchr("|_'`-,<", line[i]))
				printf("%s%c%s", ANSI_COLOR_CYAN, line[i], ANSI_COLOR_RESET);
			else
				printf("%c", line[i]);
		}
		else
		{
			if (strchr("|/'\\_'`-,<", line[i]))
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

void	*print_logo(void)
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
