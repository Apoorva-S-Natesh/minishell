/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:50:20 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 17:23:35 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
cd /: Changes to the root directory.
cd: Changes to the home directory.
cd ..: Changes to the parent directory.
cd -: Changes to the previous directory.
cd /folder/subfolder: Changes to a specified path.
how is this handled in our minishell
asomanah@c3a10c5:~$ cd core_curriculum//////so_long///maps
asomanah@c3a10c5:~/core_curriculum/so_long/maps$ - should be done in parsing or here?
*/

#include "../../includes/minishell.h"

// Function to put a string to a file descriptor 
void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

// Function to get current working directory
char	*ft_getcwd(t_shell *shell)
{
	if (getcwd(shell->cwd, 1024) == NULL)
	{
		perror("getcwd");
		free(shell->cwd);
		return (NULL);
	}
	return (shell->cwd);
}

//Function to export an environment variable
void	ft_env_export(char *var, t_env **env) // check if this function can be replaced 
{
	char	*key;
	char	*value;

	key = strtok(var, "="); //rewrite a function for strtok
	value = strtok(NULL, "=");
	append_node(env, key, value);
}

// Function to check if cd has too many arguments
static int	many_args(int num_args)
{
	if (num_args > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

//Function to change to home directory if no arguments are provided"
static int	cd_no_arg(int size, t_shell *mini)
{
	char	*home;

	if (size > 1)
		return (0);
	home = ft_getenv("HOME", mini->env);
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (-1);
	}
	else if (chdir(home) < 0)
		perror("minishell: cd");
	mini->running_status = errno;
	reutrn (mini->running_status);
}

// Function to change to the specified path
static int	cd_path(int size, char **tokens , t_shell *mini)
{
	if (size < 0)
		return (0);
	if ((strcmp(tokens[1], "~") == 0) || strcmp(tokens[1], "$1") == 0) //change strcmp to ft_strcmp
	{
		if (ft_getenv("HOME", mini->env) == NULL)
		{
			ft_putstr_fd("Minishell: cd : HOME not set\n", 2);
			return (1);
		}
		if (chdir(ft_getenv("HOME", mini->env)) < 0)
		{
			perror("minishell: cd");
			return (1);
		}
	}
	if (chdir(tokens[1]) < 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(tokens[1]);
	}
	mini->running_status = errno;
	return (mini->running_status);
}

void	builtin_cd(char **tokens, t_shell *mini, int size)
{
	static char	prev_dir[1024]; //  is declared as a static variable, meaning it retains its value between function calls.
	char 		*newpwd;

	if (many_args(size))
		return ;
	if (size == 2 && strcmp(tokens[1], "-") == 0)
	{
		if (ft_strlen(prev_dir) == 0)
		{
			ft_putstr_fd("minishel: cd: OLDPWD not set\n", 2);
			return ;
		}
		if (chdir(prev_dir) < 0)
		{
			perror("minishell: cd");
			return ;
		}
		printf("%s\n", prev_dir);
	}
	else
	{
		if (cd_no_arg(size, mini) != 0)
			return ;
		if (cd_path(size, tokens, mini) != 0)
			return ;
	}
	if (mini->cwd)
	{
		strcpy(prev_dir, mini->cwd);
		newpwd = malloc(ft_strlen("PWD=") + ft_strlen(mini->cwd) + 1);
		strcpy(newpwd, "PWD=");
		strcat(newpwd, mini->cwd);
		ft_export(newpwd, &(mini->env));
		free(newpwd);
	}
}
