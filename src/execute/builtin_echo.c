/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:58:17 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 16:13:04 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
case 1: the escape sequences
asomanah@c3a10c5:~/core_curriculum/minishell$ echo \t
t
asomanah@c3a10c5:~/core_curriculum/minishell$ echo \\t
\t
asomanah@c3a10c5:~/core_curriculum/minishell$ echo \\\t
\t
asomanah@c3a10c5:~/core_curriculum/minishell$ echo \\\t\
> k
\tk
case 1: with $
echo "$1"

echo "$USER"
user_name
bash-5.1$ echo '$USER'
$USER

case 3:  inside quotes
 echo "'\\t'"
'\t'
*/

static void	single_arg(char *arg)
{
	while (*arg)
	{
		if (*arg == '\\')
		{
			arg++;
			if (*arg == '\\')
				ft_putstr_fd("\\", STDOUT_FILENO);
			arg++;
		}
		ft_putchar_fd(*arg, STDOUT_FILENO);
		arg++;
	}
}

static int	n_flag_present(char *arg)
{
	if (!arg || ft_strlen(arg) < 2 || arg[0] != '-' || arg[1] != 'n')
		return (0);
	// while (arg[++i])
	// {
	// 	if (arg[i] != 'n')
	// 		return (0);
	// }
	return (1);
}

int	builtin_echo(char **tokens, t_shell *mini, int size) //what happens if the input has a space at the end? is it considered in the tokens?
{
	int	n_flag;
	int	i;

	if (size <= 1)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		mini->last_exit_status = 0;
		return (SUCCESS);
	}
	i = 1;
	n_flag = 0;
	while (i < size && (n_flag_present(tokens[i]))) // check if the current argument is -n flag 
	{
		n_flag = 1;
		i++;
	}
	while (tokens[i])
	{
		single_arg(tokens[i]);
		if ((i + 1) != size) //Print a space between arguments
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag) //Print newline if -n flag is not set
		ft_putstr_fd("\n", STDOUT_FILENO);
	mini->last_exit_status = 0;
	return (SUCCESS);
}
