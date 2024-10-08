/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:00:33 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 13:00:47 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_getcwd(t_shell *mini)
{
	char	*buff;
	char	*pwd;

	pwd = NULL;
	buff = NULL;
	buff = malloc(PATH_MAX * sizeof(char));
	if (!buff)
	{
		ft_putstr_fd("minishell: pwd: malloc fail\n", STDERR_FILENO);
		mini->last_exit_status = 1;
		return (NULL);
	}
	pwd = getcwd(buff, PATH_MAX);
	if (pwd == NULL)
	{
		perror("minishell: getcwd");
		mini->last_exit_status = 1;
		free(buff);
		return (NULL);
	}
	return (pwd);
}

void	builtin_pwd(t_shell *mini)
{
	char	*pwd;

	pwd = ft_getcwd(mini);
	if (pwd)
	{
		ft_putstr_fd(pwd, 1);
		ft_putstr_fd("\n", 1);
		free(pwd);
		mini->last_exit_status = 0;
	}
	else
		mini->last_exit_status = 1;
}
