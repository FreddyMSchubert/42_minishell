/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:39:03 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/22 08:17:33 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// the printouts of the validator errors.
// @Niklas
void	broadcast_validator_error(int errortype)
{
	ft_printf("Error: Invalid syntax. Errortype: %d\n", errortype);
}
