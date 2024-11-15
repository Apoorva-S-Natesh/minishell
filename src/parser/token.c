/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:25:16 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 00:54:31 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	set_redirection(char *input, int *i, t_token **tokens)
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

// Function to handle equals sign
void	handle_equals(t_tokenizer *tokenizer, int *i)
{
	tokenizer->buffer[tokenizer->buffer_index++] = tokenizer->input[*i];
	(*i)++;
	while (tokenizer->input[*i] && (tokenizer->in_quotes || \
		!ft_isspace(tokenizer->input[*i]))) 
	{
		if (tokenizer->input[*i] == '\'' || tokenizer->input[*i] == '\"')
		{
			handle_quote(tokenizer, i);
		}
		else
		{
			tokenizer->buffer[tokenizer->buffer_index++] = tokenizer->input[*i];
			(*i)++;
		}
	}
}

// Function to determine if we should break tokenization
int	should_break(char c, int in_quotes)
{
	return (!in_quotes && (c == '|' || c == '<' || c == '>'));
}

// Tokenization function
t_token	*tokenize(t_shell *mini)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (mini->input[i])
	{
		while (ft_isspace(mini->input[i]))
			i++;
		if (mini->input[i] == '\'' || mini->input[i] == '\"')
		{
			if (!handle_quotes(mini, &i, &tokens))
				return (NULL);
			continue ;
		}
		if (should_break(mini->input[i], 0))
		{
			set_redi_and_pipes(mini, &i, &tokens);
			continue ;
		}
		handle_word(mini, &i, &tokens);
	}
	return (tokens);
}
