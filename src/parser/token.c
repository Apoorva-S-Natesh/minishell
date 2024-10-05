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

void handle_word(t_shell *mini, int *i, t_token **tokens)
{
    char buffer[256];
    int buffer_index = 0;
    char *input = mini->input;
    char *quoted_content = NULL;

    // Loop through characters until you hit a delimiter (space, redirection, or pipe)
    while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
    {
        // If you encounter a quote, handle it
        if (input[*i] == '\'' || input[*i] == '\"')
        {
			if ((input[*i] == '\'' && input[(*i) + 1] == '\'') || (input[*i] == '\"' && input[(*i) + 1] == '\"'))
			{
				(*i) = (*i) + 2;
				continue ;
			}
            // Call handle_quotes to process the quoted content
            if (!handle_quotes(mini, i, tokens))
                return; // Handle unclosed quotes

            // Concatenate quoted content with the buffer (i.e., surrounding word parts)
            quoted_content = (*tokens)->value; // Get the last token's value (which was just added by handle_quotes)
            if (buffer_index > 0)
            {
                // Concatenate buffer content with quoted content
                buffer[buffer_index] = '\0'; // Null-terminate the buffer
                char *temp = ft_strjoin(buffer, quoted_content); // Join buffer and quoted content
                free((*tokens)->value); // Free old quoted content
                (*tokens)->value = temp; // Update token with concatenated string
                buffer_index = 0; // Reset buffer index
            }
            continue; // After handling the quote, continue processing
        }
        else
        {
            // Append regular characters to the buffer
            buffer[buffer_index++] = input[*i];
            (*i)++;
        }
    }

    // If a word was built in the buffer, append it as a token
    if (buffer_index > 0)
    {
        buffer[buffer_index] = '\0'; // Null-terminate the buffer
        append_token(tokens, buffer, classify_token(buffer), 0); // Append the word token
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
