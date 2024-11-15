/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:02:43 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/15 01:21:28 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	deloneenv(t_env **last, t_env *current, t_env *headcopy);
static void	ft_unset(char *key, t_env **env);

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
		if (ft_strncmp(current->key, key, len) == 0 && \
			current->key[len] == '\0')
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
		return ;
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
		{
			if (ft_strcmp(args[i], "_") != 0)
			{
				ft_unset(args[i], &mini->env);
				mini->last_exit_status = 0;
			}
		}
		i++;
	}
}
