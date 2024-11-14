/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 22:31:07 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/14 22:33:39 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_redirection(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			append_token(tokens, "<<", HEREDOC, 0);
			(*i)++;
		}
		else
			append_token(tokens, "<", RED_IN, 0);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			append_token(tokens, ">>", APPEND, 0);
			(*i)++;
		}
		else
			append_token(tokens, ">", RED_OUT, 0);
		(*i)++;
	}
}

void	set_redi_and_pipes(t_shell *mini, int *i, t_token **tokens)
{
	char	*input;

	input = mini->input;
	if (input[*i] == '|')
	{
		append_token(tokens, "|", PIPE, 0);
		(*i)++;
	}
	else if (input[*i] == '<' || input[*i] == '>')
	{
		set_redirection(input, i, tokens);
	}
}
