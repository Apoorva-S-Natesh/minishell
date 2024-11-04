/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:58:17 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/29 11:56:39 by asomanah         ###   ########.fr       */
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

static int	n_flag_present(char *arg)
{
	if (!arg || ft_strlen(arg) < 2 || arg[0] != '-' || arg[1] != 'n')
		return (0);
	return (1);
}

static void	print_single(int *i, int size, char **tokens)
{
	while (*i < size)
	{
		ft_putstr_fd(tokens[*i], STDOUT_FILENO);
		if (*i + 1 < size)
			ft_putstr_fd(" ", STDOUT_FILENO);
		(*i)++;
	}
}

int	builtin_echo(char **tokens, t_shell *mini, int size)
{
	int	n_flag;
	int	i;

	if (size <= 1)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		mini->last_exit_status = 0;
		return (SUCCESS);
	}
	i = 1;
	n_flag = 0;
	while (i < size && (n_flag_present(tokens[i])))
	{
		n_flag = 1;
		i++;
	}
	print_single(&i, size, tokens);
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	mini->last_exit_status = 0;
	return (SUCCESS);
}

/*
echo "a b c" | tr ' ' '\n' | sort | uniq:
Buffering: The most likely issue is that the output is being buffered. 
In a pipeline, stdout is redirected to a pipe, which might not be line-buffered 
by default. This can cause the output to be held in the buffer and not immediately 
sent to the next command in the pipeline.
*/