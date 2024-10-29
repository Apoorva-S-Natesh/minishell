/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:50:20 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/29 11:49:02 by asomanah         ###   ########.fr       */
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

changing directories involves more than just calling the chdir() system call. 
The shell needs to update its own internal state and environment variables (like PWD) 
to reflect the new current directory.
*/

#include "../../includes/minishell.h"

static int	cd_no_arg(t_shell *mini);
static int	cd_path(char *path, t_shell *mini);
static int	handle_cd_dash(char *prev_dir);
static int	update_direcs(char *current_dir, char *prev_dir, t_shell *mini);

//Function to change to home directory if no arguments are provided"
static int	cd_no_arg(t_shell *mini)
{
	char	*home;

	home = get_env_value("HOME", mini->env);
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
	int		result;
	int		need_free;

	result = 0;
	need_free = 0;
	if ((ft_strcmp(path, "~") == 0) || ft_strcmp(path, "$HOME") == 0)
	{
		path = get_env_value("HOME", mini->env);
		if (path == NULL)
		{
			ft_putstr_fd("Minishell: cd : HOME not set\n", 2);
			return (1);
		}
		need_free = 1;
	}
	if (chdir(path) < 0)
	{
		ft_putstr_fd("Minishell: cd : ", STDERR_FILENO);
		perror(path);
		result = 1;
	}
	if (need_free)
		free(path);
	return (result);
}

static int	handle_cd_dash(char *prev_dir)
{
	if (ft_strlen(prev_dir) == 0)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	printf("%s\n", prev_dir);
	if (chdir(prev_dir) < 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

static int	update_direcs(char *current_dir, char *prev_dir, t_shell *mini)
{
	char	new_dir[PATH_MAX];

	if (getcwd(new_dir, sizeof(new_dir)) == NULL)
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
	set_env_variable(mini, "PWD", new_dir);
	set_env_variable(mini, "OLDPWD", prev_dir);
	ft_strlcpy(mini->cwd, current_dir, sizeof(mini->cwd));
	//free(new_dir);
	return (0);
}

void	builtin_cd(char **tokens, t_shell *mini, int size)
{
	char		current_dir[PATH_MAX];
	static char	prev_dir[PATH_MAX];

	if (many_args(size))
	{
		mini->last_exit_status = 1;
		return ;
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("minishell: cd: getcwd");
		mini->last_exit_status = 1;
		return ;
	}
	if (size == 1 && (cd_no_arg(mini) != 0))
		mini->last_exit_status = 1;
	else if (size > 1 && (ft_strcmp(tokens[1], "-") == 0))
		handle_cd_dash(prev_dir);
	else if (size > 1 && (cd_path(tokens[1], mini) != 0))
		mini->last_exit_status = 1;
	else
	{
		ft_strlcpy(prev_dir, current_dir, sizeof(prev_dir));
		mini->last_exit_status = update_direcs(current_dir, prev_dir, mini);
	}
}
