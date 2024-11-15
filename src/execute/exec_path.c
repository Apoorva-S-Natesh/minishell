/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:23:57 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/15 11:14:35 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Splits the path string into an array of inidividual paths
char	**split_path(char *path)
{
	char	**paths;
	int		count;

	count = 0;
	paths = malloc(sizeof(char *) * (PATH_MAX));
	if (!paths)
		return (NULL);
	paths = ft_split(path, ':');
	return (paths);
}

//Check if command exists in any of the paths & if we have executable rights
char	*check_cmd_in_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	full_path = NULL;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// Helper function to free paths
void	free_paths(char **paths)
{
	int		i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

//Main fucntion to find the full path of a command
char	*find_command(char *cmd, t_env *variable)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value("PATH", variable);
	if (!path_env)
		return (NULL);
	paths = split_path(path_env);
	free(path_env);
	if (!paths)
		return (NULL);
	result = check_cmd_in_path(cmd, paths);
	free_paths(paths);
	return (result);
}
