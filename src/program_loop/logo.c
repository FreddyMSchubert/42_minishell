/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 18:50:02 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/27 13:44:48 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_logo_line(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ':')
			printf("%s", ANSI_COLOR_YELLOW);
		else
			printf("%s", ANSI_COLOR_CYAN);
		printf("%c", str[i]);
		printf("%s", ANSI_COLOR_RESET);
		i++;
	}
}

void	print_logo(void)
{
	print_logo_line("     ___           ___           ___         ");
	print_logo_line("  ___           ___     \n");
	print_logo_line("    /  /\\         /  /\\         /  /\\     ");
	print_logo_line("    /  /\\         /  /\\    \n");
	print_logo_line("   /  /::\\       /  /::\\       /  /::\\    ");
	print_logo_line("   /  /::\\       /  /:/    \n");
	print_logo_line("  /  /:/\\:\\     /  /:/\\:\\     /  /:/\\:\\");
	print_logo_line("     /__/:/\\:\\     /  /:/     \n");
	print_logo_line(" /  /:/  \\:\\   /  /::\\ \\:\\   /  /::\\ \\");
	print_logo_line(":\\   _\\_ \\:\\ \\:\\   /  /::\\ ___ \n");
	print_logo_line("/__/:/ \\  \\:\\ /__/:/\\:\\_\\:\\ /__/:/\\:");
	print_logo_line("\\_\\:\\ /__/\\ \\:\\ \\:\\ /__/:/\\:\\  /\\\n");
	print_logo_line("\\  \\:\\  \\__\\/ \\__\\/~|::\\/:/ \\__\\/  ");
	print_logo_line("\\:\\/:/ \\  \\:\\ \\:\\_\\/ \\__\\/  \\:\\/:/\n");
	print_logo_line(" \\  \\:\\          |  |:|::/       \\__\\::/");
	print_logo_line("   \\  \\:\\_\\:\\        \\__\\::/ \n");
	print_logo_line("  \\  \\:\\         |  |:|\\/        /  /:/  ");
	print_logo_line("   \\  \\:\\/:/        /  /:/  \n");
	print_logo_line("   \\  \\:\\        |__|:|         /__/:/    ");
	print_logo_line("   \\  \\::/        /__/:/   \n");
	print_logo_line("    \\__\\/         \\__\\|         \\__\\/  ");
	print_logo_line("       \\__\\/         \\__\\/    \n");
	printf("\n");
}
