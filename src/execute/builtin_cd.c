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
static int	cd_no_arg(t_shell *mini)
{
	char	*home;

	home = get_env_value("HOME", mini->env);
	printf("Debug: HOME directory is %s\n", home); // debug print
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (-1);
	}
	else if (chdir(home) < 0)
	{	
		perror("minishell: cd");
		return (-1);
	}
	return (0);
}

// Function to change to the specified path
static int	cd_path(char *path, t_shell *mini)
{
	char *expanded_path = path;
	printf("Debug: cd_path called with path=%s\n", path); // debug print
	if ((ft_strcmp(path, "~") == 0) || ft_strcmp(path, "$HOME") == 0) //change strcmp to ft_strcmp
	{
		expanded_path = get_env_value("HOME", mini->env);
		if (expanded_path == NULL)
		{
			ft_putstr_fd("Minishell: cd : HOME not set\n", 2);
			return (1);
		}
		printf("Debug: Expanded ~ to %s\n", expanded_path); // debug print
	}
	if (chdir(expanded_path) < 0)
	{
		ft_putstr_fd("Minishell: cd : ", STDERR_FILENO);
		perror(expanded_path);
		return (1);
	}
	return (0);
}
// 	if (chdir(tokens[1]) < 0)
// 	{
// 		printf("Debug: chdir failed for %s,\n", tokens[1]); //Debug prints
// 		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
// 		perror(tokens[1]);
// 	}
// 	mini->running_status = errno;
// 	return (mini->running_status);
// }

void	builtin_cd(char **tokens, t_shell *mini, int size)
{
	char 		current_dir[PATH_MAX];
	static char	prev_dir[PATH_MAX]; //  is declared as a static variable, meaning it retains its value between function calls.
	char 		*newpwd;

	if (many_args(size))
	{
		mini->last_exit_status = 1;
		return ;
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("minishell: cd");
		mini->last_exit_status = 1;
		return ;
	}
	if (size == 1)
	{
		if (cd_no_arg(mini) != 0)
		{
			mini->last_exit_status = 1;
			return ;
		}
	}
	else if (ft_strcmp(tokens[1], "-") == 0)
	{
		if (ft_strlen(prev_dir) == 0)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			mini->last_exit_status = 1;
			return ;
		}
		printf("%s\n", prev_dir);
		if (chdir(prev_dir) < 0)
		{
			perror("minishell: cd");
			mini->last_exit_status = 1;
			return ;
		}
	}
	else if (cd_path(tokens[1], mini) != 0)
	{
		mini->last_exit_status = 1;
		return ;
	}
	ft_strlcpy(prev_dir, current_dir, sizeof(prev_dir));
	if (getcwd(current_dir, sizeof(current_dir)) != NULL)
	{
		newpwd = ft_strjoin("PWD=", current_dir);
		set_env_variable(mini, "PWD", current_dir);
		set_env_variable(mini, "OLDPWD", prev_dir);
		ft_strlcpy(mini->cwd, current_dir, sizeof(mini->cwd));
		free(newpwd);
		printf("Debug: changed to directory: %s\n", mini->cwd); //debug print
	}
	else
		perror("minishell: cd");
	mini->last_exit_status = 0;
}

/*changing directories involves more than just calling the chdir() system call. 
The shell needs to update its own internal state and environment variables (like PWD) 
to reflect the new current directory.*/