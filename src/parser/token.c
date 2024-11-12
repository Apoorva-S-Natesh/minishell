/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:25:16 by aschmidt          #+#    #+#             */
/*   Updated: 2024/10/30 16:38:13 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void set_redirection(char *input, int *i, t_token **tokens)
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

void set_redi_and_pipes(t_shell *mini, int *i, t_token **tokens)
{
	char *input;

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

static void handle_quote(char *input, int *i, char *buffer, int *buffer_index, int *in_quotes, char *quote_char)
{
    if (!*in_quotes)
    {
        *in_quotes = 1;
        *quote_char = input[*i];
    }
    else if (input[*i] == *quote_char)
    {
        *in_quotes = 0;
        *quote_char = 0;
    }
    buffer[(*buffer_index)++] = input[*i];
    (*i)++;
}

static void handle_equals(char *input, int *i, char *buffer, int *buffer_index, int *in_quotes, char *quote_char)
{
    buffer[(*buffer_index)++] = input[*i];
    (*i)++;
    while (input[*i] && (*in_quotes || !ft_isspace(input[*i])))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
            handle_quote(input, i, buffer, buffer_index, in_quotes, quote_char);
        else
        {
            buffer[(*buffer_index)++] = input[*i];
            (*i)++;
        }
    }
}

static int should_break(char c, int in_quotes)
{
    return !in_quotes && (c == '|' || c == '<' || c == '>');
}

void handle_word(t_shell *mini, int *i, t_token **tokens)
{
    char buffer[256];
    int buffer_index = 0;
    char *input = mini->input;
    int in_quotes = 0;
    char quote_char = 0;

    while (input[*i] && (!ft_isspace(input[*i]) || in_quotes))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
            handle_quote(input, i, buffer, &buffer_index, &in_quotes, &quote_char);
        else if (input[*i] == '=' && !in_quotes)
            handle_equals(input, i, buffer, &buffer_index, &in_quotes, &quote_char);
        else if (should_break(input[*i], in_quotes))
            break;
        else
            buffer[buffer_index++] = input[(*i)++];
    }

    if (buffer_index > 0)
    {
        buffer[buffer_index] = '\0';
		char *cleaned_str = remove_quotes(buffer);
		append_or_concat_token(tokens, cleaned_str, WORD, 0);
		free(cleaned_str);
    }
}

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
				return (NULL); // Handle unclosed quotes
			continue ;
		}
		if (mini->input[i] == '|' || mini->input[i] == '<' ||
			mini->input[i] == '>')
		{
			set_redi_and_pipes(mini, &i, &tokens);
			continue ;
		}
		handle_word(mini, &i, &tokens);
	}
	return (tokens);
}
