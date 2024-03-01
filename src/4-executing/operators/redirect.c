/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:07:34 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/01 17:07:32 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_arguments_after_redirection(t_bin_tree_node *node)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (node->l->val[i])
		j++;
	while (node->r->val[++i] != NULL)
	{
		node->r->val[i] = node->r->val[j++];
	}
	
}

int	redirect(t_bin_tree_node *node, t_program_data *program_data)
{
	int		fd;
	int		flags;
	// int		perm;

	if (node->input_fd != 0)
		node->l->input_fd = node->input_fd;
	if (node->output_fd != 1)
		node->l->output_fd = node->output_fd;
	// printf("node->val[0]->value: %s\n", node->val[1]->value);
	if (ft_strncmp(node->val[0]->value, "<", 1) == 0)
		flags = O_RDONLY;
	else if (ft_strncmp(node->val[0]->value, ">>", 2) == 0)
		flags = O_APPEND | O_WRONLY | O_CREAT;
	else if (ft_strncmp(node->val[0]->value, ">", 1) == 0)
		flags = O_TRUNC | O_WRONLY | O_CREAT;
	if (node->val[0]->value[0] == '<' || node->val[0]->value[1] == '>')
		fd = open(node->r->val[0]->value, flags, 0644);
	else
		fd = open(node->r->val[0]->value, flags, 0644);
	if (fd < 0)
	{
		ft_printf("minishell: %s: %s\n", node->r->val[0]->value, strerror(errno));
		return (-1);
	}
	if (ft_strncmp(node->val[0]->value, "<", 2) == 0)
		node->l->input_fd = fd;
	else
		node->l->output_fd = fd;
	(void)program_data;
	return (0);
}
