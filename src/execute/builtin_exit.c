/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:01:38 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/30 17:06:32 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
No arguments: exits with the status of the last executed command.
One numeric argument: exits with that status (adjusted to be between 0-255).
Non-numeric argument: prints an error and exits with status 255.
More than one argument: prints an error message and doesn't exit.
*/

#include "../../includes/minishell.h"

static char	is_valid_exit_arg(const char *arg)
{
	if (*arg == '-' || *arg == '+')
		arg++;
	if (*arg == '\0')
		return (0);
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (0);
		arg++;
	}
	return (1);
}

static void	print_exit_error(const char *arg, const char *message)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

void	builtin_exit(char **tokens, t_shell *mini, int size)
{
	int	exit_status;

	exit_status = mini->last_exit_status;
	ft_putstr_fd("exit\n", 1);
	if (size > 1)
	{
		if (!is_valid_exit_arg(tokens[1]))
		{
			print_exit_error(tokens[1], "numeric argument required");
			exit_status = 2;
		}
		else
		{
			exit_status = ft_atoi(tokens[1]);
			if (size > 2)
			{
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
				mini->last_exit_status = 1;
				return ;
			}
		}
	}
	free_command(mini->commands);
	free_list(mini->env);
	free_tokens(mini->token);
	exit(exit_status);
}
