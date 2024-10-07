#include "../../includes/minishell.h"

//Splits the path string into an array of inidividual paths
char	**split_path(char *path)
{
	char	**paths;
	char	*token;
	int		count;

	count = 0;
	paths = malloc(sizeof(char *) * (PATH_MAX / 2));
	if (!paths)
		return (NULL);
	token = ft_strtok(path, ":");
	while (token)
	{
		paths[count] = ft_strdup(token);
		count++;
		token = ft_strtok(NULL, ":");
	}
	paths[count] = NULL;
	return (paths);
}

//Checks if the command exists in any of the paths and if we have executable rights
char	*check_cmd_in_path(char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	i = 0;
	full_path = NULL;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], cmd);
		if (access(full_path, X_OK) == 0) //checks if file exits and if has executable permission
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

//Main fucntion to find the full path of a command
char	*find_command(char *cmd, t_env *variable)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = ft_getenv("PATH", variable);
	if (!path_env)
		return (NULL);
	paths = split_path(path_env);
	free(path_env);
	if (!paths)
		return (NULL);
	result = check_cmd_in_path(cmd, paths);
	i = 0;
	while (paths[i]) //Freing paths array
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return (result);
}
