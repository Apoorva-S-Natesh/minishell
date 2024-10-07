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

void	handle_env_expansion(char *arg, t_env *env) //Delete function if already present 
{
	char	var_name[256];
	char	*env_value;
	int		var_len;

	var_len = 0;
	arg++;
	env_value = NULL;
	//Extract the variable name (alphanumeric or underscore)
	while(ft_isalnum(*arg) || *arg == '_')
	{
		var_name[var_len++] = *arg;
		arg++;
	}
	var_name[var_len] = '\0'; //Null terminate the variable name
	env_value = ft_getenv(var_name, env);
	if (env_value)
		ft_putstr_fd(env_value, STDOUT_FILENO);
}

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
			write(STDOUT_FILENO, arg, 1);
		}
		arg++;
	}
}

static int	n_flag_present(char *arg)
{
	int	i;

	i = 0;
	if (!arg || ft_strlen(arg) < 2 || arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[++i])
	{
		if (arg[i] != 'n')
			return (0);
	}
	return (1);
}

int	builtin_echo(char **tokens, t_shell *mini, int size) //what happens if the input has a space at the end? is it considered in the tokens?
{
	int	n_flag;
	int	i;

	if (!tokens && !*tokens)
		return (1);
	i = 0;
	n_flag = 0;
	while (++i < size && (n_flag_present(tokens[i]))) // check if the current argument is -n flag 
		n_flag = 1;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
			handle_env_expansion(tokens[i], mini->env); //Delete this part of code when the expansion is done in the parsing stage
		else
			single_arg(tokens[i]);
		if ((i + 1) != size) //Print a space between arguments
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag) //Print newline if -n flag is not set
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}
