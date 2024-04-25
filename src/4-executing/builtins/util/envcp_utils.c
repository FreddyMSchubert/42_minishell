/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envcp_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:17:01 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 10:35:33 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

// function to get the value of an environment variable
// gets a reference to the actual environment variable, 
// so strdup it if you want to keep it
char	*get_envcp_var(char *var, char **envcp)
{
	int	i;
	int	j;
	int	var_i;

	i = 0;
	while (envcp[i])
	{
		j = 0;
		var_i = 0;
		while (envcp[i][j] && var[var_i] && envcp[i][j] == var[var_i])
		{
			j++;
			var_i++;
		}
		if (var[var_i] == '\0' && envcp[i][j] == '=')
			return (envcp[i] + j + 1);
		i++;
	}
	return (NULL);
}

// creates a new environment variable with the given value
int	create_envcp_var(char *var, char *value, t_program_data *data)
{
	char	*newenvcp;
	char	**temp;
	int		i;

	newenvcp = ft_strjoinfree(ft_strjoin(var, "="), ft_strdup(value));
	if (!newenvcp)
		return (-1);
	i = 0;
	while (data->envcp[i])
		i++;
	temp = (char **)realloc(data->envcp, sizeof(char *) * (i + 2)); // forbidden function
	if (!temp)
		return (free(newenvcp), -2);
	data->envcp = temp;
	data->envcp[i] = newenvcp;
	data->envcp[i + 1] = NULL;
	return (0);
}

// function to set the value of an envcpironment variable
// if it fails to find the variable and createnew is 1, it creates a new one
int	set_envcp_var(char *var, char *val, char createnew, t_program_data *data)
{
	int			i;
	int			k;

	if (val == NULL)
		val = "";
	i = -1;
	while (data->envcp[++i])
	{
		k = -1;
		while (data->envcp[i][++k] && var[k] && data->envcp[i][k] == var[k])
			;
		if (var[k] == '\0' && data->envcp[i][k] == '=')
		{
			free(data->envcp[i]);
			data->envcp[i] = ft_strjoinfree(ft_strjoin(var, "="), \
											ft_strdup(val));
			if (!data->envcp[i])
				return (-1);
			return (0);
		}
	}
	if (createnew == 1)
		return (create_envcp_var(var, val, data));
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
		while (envcp[i][j] && var[k] && envcp[i][j++] == var[k])
			k++;
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
	return (-1);
}
