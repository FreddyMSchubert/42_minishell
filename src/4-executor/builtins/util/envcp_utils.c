/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envcp_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 07:17:01 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 17:06:53 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

/*
	@brief	gets the value of an environment variable
			returns a reference, so strdup before modifying
	@param	var		name of the variable
	@param	envcp	pointer to the environment variables array
	@return	pointer to the value of the variable
			NULL	if the variable is not found
*/
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

/*
	@brief	creates a new environment variable
	@param	var		name of the variable
	@param	value	value of the variable
	@param	data	pointer to the data struct
	@return	0		success
			-1		malloc failed
			-2		malloc failed
*/
int	create_envcp_var(char *var, char *value, t_data *data)
{
	char	*newenvcp;
	char	**new_envcp_array;
	int		i;
	int		j;

	newenvcp = ft_strjoinfree(ft_strjoin(var, "="), ft_strdup(value));
	if (!newenvcp)
		return (-1);
	i = 0;
	while (data->envcp[i])
		i++;
	new_envcp_array = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_envcp_array)
		return (free(newenvcp), -2);
	j = -1;
	while (++j < i)
		new_envcp_array[j] = data->envcp[j];
	new_envcp_array[i] = newenvcp;
	new_envcp_array[i + 1] = NULL;
	free(data->envcp);
	data->envcp = new_envcp_array;
	return (0);
}

/*
	@brief	sets the value of an environment variable
	@param	var			name of the variable
	@param	val			value of the variable
	@param	createnew	1 if the variable should be created if it doesn't exist
	@param	data		pointer to the data struct
	@return	0		success
			-1		malloc failed
			-2		variable not found and createnew is 0
*/
int	set_envcp_var(char *var, char *val, char createnew, t_data *data)
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

/*
	@brief	deletes an environment variable
	@param	var		name of the variable
	@param	envcp	pointer to the environment variables array
	@return	0		success
			-1		variable not found
*/
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
