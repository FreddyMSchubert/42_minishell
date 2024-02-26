/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/26 07:58:11 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// returns a string with the path of the parent directory from an absolute path
// expects the last character to not be a slash
// if we are at root, return "/"
// example: /home/user/ -> /home/
char	*go_up_one_directory(char *path)
{
	int	i;
	int	last_slash;

	i = 0;
	last_slash = -1;
	while (path[i])
	{
		if (path[i] == '/' && i != (int)ft_strlen(path) - 1)
			last_slash = i;
		i++;
	}
	if (last_slash == 0) // if we are at root
		return (ft_strdup("/"));
	if (last_slash == -1) // if there is no slash (this shouldnt happen)
		return (ft_strdup("."));
	path[last_slash] = '\0';
	return (path);
}

// split path at '/' and loop through each part
// free rel_path, dont touch currdirpath as its part of the env
char	*get_absolute_path(char *rel_path, char *currdirpath)
{
	char	*currdir;
	char	**split_rel_path;
	int		i;

	split_rel_path = ft_split(rel_path, '/');
	free(rel_path);
	currdir = ft_strdup(currdirpath);
	i = -1;
	while (split_rel_path[++i])
	{
		if (ft_strncmp(split_rel_path[i], "..", 2) == 0)
			currdir = go_up_one_directory(currdir);
		else if (ft_strncmp(split_rel_path[i], ".", 1) == 0)
			currdir = ft_strjoinfree(currdir, ft_strdup("/"));
		else
		{
			currdir = ft_strjoinfree(currdir, ft_strdup("/"));
			currdir = ft_strjoinfree(currdir, ft_strdup(split_rel_path[i]));
		}
	}
	i = -1;
	while (split_rel_path[++i])
		free(split_rel_path[i]);
	free(split_rel_path);
	return (currdir);
}

int	execute_cd(t_token **tokens, t_program_data *program_data)
{
	char	*path;
	char	*buffer;
	char	*home;
	int		ret_val;

	if (tokens[1] == NULL)
	{
		home = get_envcp_var("HOME", program_data->envcp);
		if (home)
			path = ft_strdup(home);
		else
			return (-1); // handle error
	}
	else
		path = ft_strdup(tokens[1]->value);
	ret_val = chdir(path);
	if (ret_val != 0)
	{
		broadcast_builtin_error("cd", -4, NULL);
		return (errno);
	}
	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (-1); // handle error
	if (set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), \
														0, program_data) == -1)
		return (broadcast_builtin_error("cd", -2, NULL), free(path), 1);
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
		return (broadcast_builtin_error("cd", -2, "PWD"), free(path), 1);
	free(path);
	return (0);
}
