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

void	builtin_exit(char **tokens, t_shell *mini, int size)
{
	
}

/*
static int	too_many_args(int size)
{
	if (size > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

static int	is_numeric_arg(char **cmdarr)
{
	int	i;

	i = -1;
	if (ft_isplusminus(cmdarr[1][0]))
		++i;
	while (cmdarr[1][++i])
	{
		if (!ft_isdigit(cmdarr[1][i]) || i > 9)
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmdarr[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (0);
		}
	}
	return (1);
}

// if just 'exit' it actually carries the errno when no args. See: bbbb || exit
// if too_many_args, Bash too just complains, prints exit, but doesn't exit
int	ft_exit(int size, char **cmdarr)
{
	if (!cmdarr && !cmdarr[0])
		return (1);
	ft_putstr_fd("exit\n", 2);
	if (size == 1)
		exit(errno);
	if (too_many_args(size))
		return (1);
	if (!is_numeric_arg(cmdarr))
		exit(2);
	exit((unsigned char)ft_atoi(cmdarr[1]));
	return (SUCCESS);
}
*/

/*
chat gpt
int ft_exit(int size, char **cmdarr, t_shell *mini)
{
    // Print exit message
    ft_putstr_fd("exit\n", STDOUT_FILENO);

    // If no arguments, exit with the last exit status
    if (size == 1)
        exit(mini->last_exit_status);  // Use last exit status stored in t_shell

    // If too many arguments, print an error and return 1 without exiting
    if (too_many_args(size))
        return 1;

    // If the argument is not a numeric value, exit with status 2
    if (!is_numeric_arg(cmdarr[1]))
        exit(2);

    // Convert the argument to an exit status and exit
    int exit_status = ft_atoi(cmdarr[1]);
    exit((unsigned char)exit_status);
}
*/