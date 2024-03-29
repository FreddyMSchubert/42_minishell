/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envcp_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:17:01 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/27 10:10:49 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

// function to get the value of an environment variable
// gets a reference to the actual environment variable, 
// so strdup it if you want to keep it
char	*get_envcp_var(char *var, char **envcp)
{
	int	env_i;
	int	env_i_i;
	int	var_i;

	env_i = 0;
	while (envcp[env_i])
	{
		env_i_i = 0;
		var_i = 0;
		while (envcp[env_i][env_i_i] && var[var_i] && envcp[env_i][env_i_i] == var[var_i])
		{
			env_i_i++;
			var_i++;
		}
		if (var[var_i] == '\0' && envcp[env_i][env_i_i] == '=')
			return (envcp[env_i] + env_i_i + 1);
		env_i++;
	}
	ft_printf("there is no path!\n");
	return (NULL);
}

// creates a new environment variable with the given value
int	create_envcp_var(char *var, char *value, char **envcp, \
					t_program_data *program_data)
{
	char	*newenvcp;
	char	**temp;
	int		i;

	newenvcp = ft_strjoinfree(ft_strjoin(var, "="), ft_strdup(value));
	if (!newenvcp)
		return (-1);
	i = 0;
	while (envcp[i])
		i++;
	temp = (char **)realloc(envcp, sizeof(char *) * (i + 2));
	if (!temp)
		return (free(newenvcp), -2);
	program_data->envcp = temp;
	envcp = temp;
	envcp[i] = newenvcp;
	envcp[i + 1] = NULL;
	return (0);
}

// function to set the value of an envcpironment variable
// if it fails to find the variable and createnew is 1, it creates a new one
int	set_envcp_var(char *var, char *val, char createnew, t_program_data *data)
{
	int	i;
	int	j;
	int	k;

	if (val == NULL)
		val = "";
	i = -1;
	while (data->envcp[++i])
	{
		j = 0;
		k = 0;
		while (data->envcp[i][j] && var[k] && data->envcp[i][j++] == var[k++])
			;
		if (var[k] == '\0' && data->envcp[i][j] == '=')
		{
			free(data->envcp[i]);
			data->envcp[i] = ft_strjoinfree(ft_strjoin(var, "="), \
											ft_strdup(val));
			if (!data->envcp[i])
				return (-1); // handle error
			return (0);
		}
	}
	if (createnew)
		return (create_envcp_var(var, val, data->envcp, data));
	return (-2);
}

int	delete_envcp_var(char *var, char **envcp)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	while (envcp[++i])
	{
		j = 0;
		k = 0;
		while (envcp[i][j] && var[k] && envcp[i][j++] == var[k++])
			;
		if (var[k] == '\0' && envcp[i][j] == '=')
		{
			free(envcp[i]);
			while (envcp[i])
			{
				envcp[i] = envcp[i + 1];
				i++;
			}
			return (0);
		}
	}
	return (broadcast_builtin_error("cd", -3, var), -1);
}
