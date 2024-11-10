/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:12:50 by aschmidt          #+#    #+#             */
/*   Updated: 2024/10/30 21:52:49 by asomanah         ###   ########.fr       */
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
	mini->last_exit_status = 0;
	mini->expand_heredoc = 0;
	set_envv(mini, envv);
}

// void	set_envv(t_shell *mini, char **envv)
// {
// 	int		i;
// 	t_env	*head;
// 	char	**split_envv;

// 	i = 0;
// 	head = NULL;
// 	while (envv[i] != NULL)
// 	{
// 		split_envv = ft_split(envv[i], '=');
// 		if (!split_envv)
// 		{
// 			ft_putstr_fd("set_envv Memory allocation failed\n", STDERR_FILENO);
// 			return ;
// 		}
// 		if (split_envv[0] && split_envv[1])
// 			append_node(&head, split_envv[0], split_envv[1]);
// 		ft_free(split_envv);
// 		i++;
// 	}
// 	mini->env = head;
// }

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
			*equals_pos = '\0';  // Temporarily split the string
			append_node(&head, envv[i], equals_pos + 1);
			*equals_pos = '=';  // Restore the original string
		}
		i++;
	}
	mini->env = head;
}
