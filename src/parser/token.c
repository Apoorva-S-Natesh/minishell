/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:25:16 by aschmidt          #+#    #+#             */
/*   Updated: 2024/10/03 15:48:11 by aschmidt         ###   ########.fr       */
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

static int process_quoted_content(t_shell *mini, int *i, t_token **tokens, char *buffer, int *buffer_index)
{
    char *quoted_content;

    if (!handle_quotes(mini, i, tokens))
        return 0;  // Handle unclosed quotes

    quoted_content = (*tokens)->value;  // Get the quoted content just processed
    if (*buffer_index > 0)
    {
        // Concatenate buffer content with quoted content
        buffer[*buffer_index] = '\0';
        char *temp = ft_strjoin(buffer, quoted_content);  // Join buffer and quoted content
        free((*tokens)->value);  // Free old quoted content
        (*tokens)->value = temp;  // Update token with concatenated string
        *buffer_index = 0;  // Reset buffer index
    }

    return 1;
}

void handle_word(t_shell *mini, int *i, t_token **tokens)
{
    char buffer[256];
    int buffer_index;
    char *input;

	buffer_index = 0;
	input = mini->input;
    while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
    {
        if (input[*i] == '\'' || input[*i] == '\"')
        {
			if ((input[*i] == '\'' && input[(*i) + 1] == '\'') || (input[*i] == '\"' && input[(*i) + 1] == '\"'))
			{
				(*i) += 2;
				continue;
			}
            if (!process_quoted_content(mini, i, tokens, buffer, &buffer_index))
                return;  // Handle unclosed quotes or concatenation
            continue;
        }
        else
        {
            buffer[buffer_index++] = input[*i];
            (*i)++;
        }
    }
    if (buffer_index > 0)
    {
        buffer[buffer_index] = '\0'; // Null-terminate the buffer
		if(tokens)
        	append_or_concat_token(tokens, buffer, WORD, 0); // Append the word token
		else
			append_token(tokens, buffer, WORD, 0);
		set_concat_flag(input, *i, get_last_token(*tokens));
    }
}

t_token *tokenize(t_shell *mini)
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
		if (mini->input[i] == '|' || mini->input[i] == '<' || mini->input[i] == '>')
		{
			set_redi_and_pipes(mini, &i, &tokens);
			continue;
		}
		handle_word(mini, &i, &tokens);
	}
	return (tokens);
}
