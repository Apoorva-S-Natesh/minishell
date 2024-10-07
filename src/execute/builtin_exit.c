/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:01:38 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 17:20:57 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
case 1: too many arguments and doesnt exit
exit 1 3 45 a
exit
bash: exit: too many arguments 
case 2: Exits but says needs numeric arguments
exit aaaaaa
exit
bash: exit: aaaaaa: numeric argument required
case 3: exit with no argument or numeric argument
exits
*/

#include "../../includes/minishell.h"

static char	is_valid_exit_arg(const char *arg)
{
	if (*arg == '-' || *arg == '+')
		arg++;
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

static int	parse_exit_status(const char *arg)
{
	long	num;

	num = ft_atoi(arg);
	if (num < 0 || num > 255) //Check if the number is out of hte valid range for exit status
	{
		num %= 256;
		if (num < 0)
			num += 256; 
	}
	return ((int)num);
}

void	builtin_exit(char **tokens, t_shell *mini, int size)
{
	int	exit_status;

	exit_status = mini->last_exit_status;
	ft_putstr_fd("exit\n", 2);
	if (size > 1)
	{
		if (!is_valid_exit_arg(tokens[1]))
		{
			print_exit_error(tokens[1], "numeric argument required");
			exit_status = 2;
		}
		else
		{
			exit_status = parse_exit_status(tokens[1]);
			if (size > 2)
			{
				ft_putstr_fd("minishell: exit: too many arguments\n", 2);
				mini->last_exit_status = 1;
				return ;
			}
		}
	}
	//Clean up resources
	//free resources(mini);
	exit(exit_status);
}

/*
No arguments: exits with the status of the last executed command.
One numeric argument: exits with that status (adjusted to be between 0-255).
Non-numeric argument: prints an error and exits with status 255.
More than one argument: prints an error message and doesn't exit.
*/