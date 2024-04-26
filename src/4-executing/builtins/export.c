/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:38:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:34:11 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	export_err(int error_code, char *arg, t_data *data)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	if (error_code == 2)
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	else if (error_code == 3)
		ft_putstr_fd(": error setting env variable\n", STDERR_FILENO);
	else if (error_code == 4)
		ft_putstr_fd(": error allocating memory\n", STDERR_FILENO);
	data->exit_status = 1;
	return (1);
}

// checks if the variable is a valid env var
static bool	valid_var(char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]) && var[i] != '_')
		return (false);
	while (var[++i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
	}
	return (true);
}

static int	display_all_env_vars(t_data *sh, int out_fd)
{
	int	i;

	i = 0;
	while (sh->envcp[i])
	{
		ft_putstr_fd("declare -x ", out_fd);
		ft_putendl_fd(sh->envcp[i], out_fd);
		i++;
	}
	return (0);
}

static int	parse_and_set_env_var(t_tok *token, t_data *sh)
{
	char	*equ_sign;
	char	*var;
	char	*val;

	equ_sign = ft_strchr(token->val, '=');
	if (!equ_sign || equ_sign == token->val || !valid_var(token->val))
	{
		if (equ_sign == token->val || !valid_var(token->val))
			return (export_err(2, token->val, sh));
		return (0);
	}
	var = ft_substr(token->val, 0, equ_sign - token->val);
	if (!var)
		return (export_err(4, NULL, sh));
	if (ft_isnbr(var))
		return (free(var), export_err(2, token->val, sh));
	val = ft_substr(token->val, equ_sign - token->val + 1, \
						ft_strlen(token->val) - (equ_sign - token->val) - 1);
	if (!val)
		return (free(var), export_err(4, NULL, sh));
	if (set_envcp_var(var, val, 1, sh) != 0)
		return (free(var), free(val), export_err(3, NULL, sh));
	free(var);
	free(val);
	return (0);
}

int	execute_export(t_tok **toks, int out_fd, t_data *sh)
{
	int		i;
	int		result;

	if (!toks[1])
		return (display_all_env_vars(sh, out_fd), 0);
	i = 0;
	while (toks[++i])
	{
		result = parse_and_set_env_var(toks[i], sh);
		if (result != 0)
			return (result);
	}
	return (sh->exit_status);
}
