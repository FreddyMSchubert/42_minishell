/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:21:20 by nburchha          #+#    #+#             */
/*   Updated: 2024/04/27 10:28:44 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_expander(char **input, t_data *sh)
{
	*input = expand(*input, sh, false);
	if (VERBOSE == 1)
		printf("Expanded input: %s\n", *input);
	return (0);
}

int	execute_lexer(char *input, t_data *sh, t_list **tokenified_input)
{
	*tokenified_input = lexer(input, sh);
	if (*tokenified_input == NULL)
		return (-1);
	if (VERBOSE == 1)
	{
		printf("Lexed input: ");
		print_tokens(*tokenified_input);
	}
	return (0);
}

int	execute_validator(t_list *tokenified_input, t_data *sh)
{
	int	valid;

	valid = validate(tokenified_input);
	sh->exit_status = valid;
	if (valid != 0)
	{
		if (VERBOSE == 1)
			printf("Validated input: input is invalid (%d)\n", valid);
		if (!isatty(fileno(stdin)))
			sh->exit_flag = 1;
		gc_clean_and_reinit(&sh->gc);
		return (-1);
	}
	if (VERBOSE == 1)
		printf("Validated input: input is valid\n");
	return (0);
}

int	execute_parser(t_list *tokenified_input, t_data *sh, t_node **tree)
{
	*tree = parse(tokenified_input, sh);
	if (*tree == NULL)
		return (-1);
	(*tree)->parent = NULL;
	if (VERBOSE == 1)
	{
		printf("Parsed input: ");
		print_node_with_children(*tree, 0);
		printf("\n");
	}
	return (0);
}

int	execute_executor(t_node *tree, t_data *sh)
{
	t_pid_list	*pid_list;

	pid_list = NULL;
	execute(tree, sh, &pid_list);
	resolve_pid_list(sh, &pid_list);
	return (0);
}
