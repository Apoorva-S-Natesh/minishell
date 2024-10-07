/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:02:43 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 13:02:46 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	deloneenv(t_env **last, t_env *current, t_env *headcopy)
{
	if (!last || !(*last) || !current)
		return ;

	if (*last != current)
	{
		(*last)->next = current->next;
		*last = headcopy;
	}
	else
		*last = current->next;
	free(current->key);
	free(current->value);
	free(current);
}

static void	ft_unset(char *key, t_env **env)
{
	int		len;
	t_env	*headcopy;
	t_env	*current;

	if (!key || !env || !(*env))
		return ;
	len = ft_strlen(key);
	headcopy = *env;
	current = *env;
	while (current)
	{
		if (strncmp(current->key, key, len) == 0 && current->key[len] == '\0')
		{
			deloneenv(env, current, headcopy);
			return ;
		}
		*env = current;
		current = current->next;
	}
	*env = headcopy;
}

void	builtin_unset(t_shell *mini, char **args)
{
	int	i;

	i = 1;
	if (!args[1])
		return ; // No arguments provided, do nothing (POSIX compliant)
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
		{
			if (strcmp(args[i], "_") != 0) // Cannot unset "_" (POSIX requirement)
			{
				ft_unset(args[i], &mini->env);
				mini->last_exit_status = 0;
			}
		}
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			mini->last_exit_status = 1;
		}
		i++;
	}
}
