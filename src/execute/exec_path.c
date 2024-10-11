#include "../../includes/minishell.h"

//Splits the path string into an array of inidividual paths
char	**split_path(char *path)
{
	char	**paths;
	char	*token;
	int		count;

	count = 0;
	paths = malloc(sizeof(char *) * (PATH_MAX));
	if (!paths)
		return (NULL);
	token = strtok(path, ":");
	while (token)
	{
		printf("Path component: %s\n", token);
		paths[count] = ft_strdup(token);
		count++;
		token = strtok(NULL, ":");
	}
	paths[count] = NULL;
	return (paths);
}

//Checks if the command exists in any of the paths and if we have executable rights
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
		printf("checking path: %s\n", full_path);
		if (access(full_path, X_OK) == 0)
		{
			printf("Found executable: %s\n", full_path);
			return (full_path);
		}
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
	print_list(variable);
	path_env = get_env_value("PATH", variable);
	printf("PATH env variable is: %s\n", path_env);
	if (!path_env)
		return (NULL);
	paths = split_path(path_env);
	free(path_env);
	if (!paths)
		return (NULL);
	 // Debug print: print all paths
    for (i = 0; paths[i]; i++)
    {
		printf("Path %d: %s\n", i, paths[i]);
	}
	result = check_cmd_in_path(cmd, paths);
	i = 0;
	while (paths[i]) //Freeing paths array
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	 printf("Found command path: %s\n", result);  // Debug print
	return (result);
}
