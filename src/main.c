/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/25 09:54:34 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_minishell(t_shell *mini, t_token *tokens)
{
	expand_tokens(tokens, mini);
	print_tokens(tokens);
	mini->commands = group_tokens_to_cmd(tokens);
	set_cmd_priorities(mini->commands);
	print_commands(mini->commands);
	execute(mini);
	free_tokens(tokens);
	free_command(mini->commands);
}

int	main(int ac, char **av, char **envv)
{
	t_shell	mini;
	t_token	*tokens;

	if (!check_args(ac, av))
		return (0);
	init_shell(&mini, envv);
	setup_sig_handling(&mini);
	while (mini.running_status)
	{
		tokens = NULL;
		if (take_input(&mini))
		{
			tokens = tokenize(&mini);
			if (!tokens)
				continue ;
			else
				execute_minishell(&mini, tokens);
		}
	}
	free_list(mini.env);
	return (0);
}

/*
Ctrl-D is not actually a signal, but is handled by the readline library as an EOF condition.
You should handle this in your take_input function. If readline returns NULL, it
means EOF was encountered (Ctrl-D was pressed).*/
