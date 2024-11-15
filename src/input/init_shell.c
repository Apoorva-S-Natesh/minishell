/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:12:50 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/14 19:21:38 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_shell(t_shell *mini, char **envv)
{
	mini->running_status = 1;
	mini->input = NULL;
	mini->commands = NULL;
	mini->last_exit_status = 0;
	mini->expand_heredoc = 0;
	set_envv(mini, envv);
}

void	set_envv(t_shell *mini, char **envv)
{
	int		i;
	t_env	*head;
	char	*equals_pos;

	i = 0;
	head = NULL;
	while (envv[i] != NULL)
	{
		equals_pos = ft_strchr(envv[i], '=');
		if (equals_pos)
		{
			*equals_pos = '\0';
			append_node(&head, envv[i], equals_pos + 1);
			*equals_pos = '=';
		}
		i++;
	}
	mini->env = head;
}
