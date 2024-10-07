/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:12:50 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/23 15:26:50 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}*/

void	init_shell(t_shell *mini, char **envv)
{
	mini->running_status = 1;
	mini->input = NULL;
	mini->commands = NULL;
	set_envv(mini, envv);
}

void	set_envv(t_shell *mini, char **envv)
{
	int		i;
	t_env	*head;
	char	**split_envv;

	i = 0;
	head = NULL;
	while (envv[i] != NULL)
	{
		split_envv = ft_split(envv[i], '=');
		if (split_envv[0] && split_envv[1])
			append_node(&head, split_envv[0], split_envv[1]);
		ft_free(split_envv);
		i++;
	}
	mini->env = head;
}
