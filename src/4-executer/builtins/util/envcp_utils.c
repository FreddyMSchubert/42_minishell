/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envcp_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:17:01 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/21 10:38:26 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

// function to get the value of an environment variable
// gets a reference to the actual environment variable, so strdup it if you want to keep it
char *get_envcp_var(char *var, char **envcp)
{
	int i;	// index of envcp
	int j;	// index of envcp[i]
	int k;	// index of var

	i = 0;
	while (envcp[i])
	{
		j = 0;
		k = 0;
		while (envcp[i][j] && var[k] && envcp[i][j] == var[k])
		{
			j++;
			k++;
		}
		if (var[k] == '\0' && envcp[i][j] == '=')
			return (envcp[i] + j + 1);
		i++;
	}
	return (NULL);
}

// creates a new environment variable with the given value
int create_envcp_var(char *var, char *value, char **envcp, t_program_data *program_data)
{
	char *newenvcp;
	char **temp;
	int i;

	newenvcp = ft_strjoinfree(ft_strjoin(var, "="), ft_strdup(value));
	if (!newenvcp) return (-1);
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
int set_envcp_var(char *var, char *value, char createnew, t_program_data *program_data)
{
	int i;	// index of envcp
	int j;	// index of envcp[i]
	int k;	// index of var

	i = -1;
	while (program_data->envcp[++i])
	{
		j = 0;
		k = 0;
		while (program_data->envcp[i][j] && var[k] && program_data->envcp[i][j] == var[k])
		{
			j++;
			k++;
		}
		if (var[k] == '\0' && program_data->envcp[i][j] == '=')
		{
			free(program_data->envcp[i]);
			program_data->envcp[i] = ft_strjoinfree(ft_strjoin(var, "="), ft_strdup(value));
			if (!program_data->envcp[i])
				return (-1); // handle error
			return (0);
		}
	}
	if (createnew)
		return (create_envcp_var(var, value, program_data->envcp, program_data));
	return (broadcast_builtin_error("cd", -3, var), -1);
}

int delete_envcp_var(char *var, char **envcp)
{
	int i;	// index of envcp
	int j;	// index of envcp[i]
	int k;	// index of var

	i = -1;
	while (envcp[++i])
	{
		j = 0;
		k = 0;
		while (envcp[i][j] && var[k] && envcp[i][j] == var[k])
		{
			j++;
			k++;
		}
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