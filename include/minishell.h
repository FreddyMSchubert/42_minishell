/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/01/30 11:30:35 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ----- SUBMODULES

#include "../submodules/42_ft_printf/ft_printf.h"
#include "../submodules/42_libft/libft.h"
#include "../submodules/42_get_next_line/get_next_line.h"

// ----- SETTINGS

// will output detailed logging if set to 1, and normal logging if 0
#define VERBOSE 0