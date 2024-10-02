/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:02:30 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 13:02:32 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	builtin_env(char **tokens, t_shell *mini, int size)
{
	t_env	*current;

	if (size > 1)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		mini->last_exit_status = 1;
		return ;
	}
	current = mini->env;
	while (current != NULL)
	{
		if (current->value && (current->value[0] != '\0')) //Only print if the variable has a value
		{
			ft_putstr_fd(current->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\n", 1);
		}
		current = current->next;
	}
	mini->last_exit_status = 0;
}
