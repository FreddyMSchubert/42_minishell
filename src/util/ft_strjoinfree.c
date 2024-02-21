/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:25:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/21 08:56:30 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// runs strjoin but frees both inputs.
// if you dont want your input freed, pass ft_strdup(input)
char	*ft_strjoinfree(char *s1, char *s2)
{
	char	*p;

	p = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (p);
}
