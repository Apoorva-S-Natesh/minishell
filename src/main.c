/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 11:14:55 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_minishell(t_shell *mini, t_token *tokens)
{
	int	validate_result;

	expand_tokens(&tokens, mini);
	validate_result = validate_tokens(tokens, mini);
	if (validate_result == -1)
		printf("syntax error near unexpected token `|'\n");
	else if (validate_result == -2)
		printf("syntax error near unexpected token `newline'\n");
	if (validate_result < 0)
	{
		free_tokens(tokens);
		return ;
	}
	mini->token = tokens;
	mini->commands = group_tokens_to_cmd(tokens);
	set_cmd_priorities(mini->commands);
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
		restore_main_signals();
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
	cleanup_signal_pipe(&mini);
	free_list(mini.env);
	return (0);
}
