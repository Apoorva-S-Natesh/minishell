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

void handle_quotes(char *input, int *i, t_token **tokens)
{
    char buffer[256]; // Temporary buffer for token storage
    int buffer_index;
    int quote_type;

    buffer_index = 0;
    if (input[*i] == '\'')
		quote_type = 1;  // Single quote
	else if (input[*i] == '\"')
		quote_type = 2;  // Double quote
    (*i)++; // Move past the quote
    while (input[*i] && input[*i] != (quote_type == 1 ? '\'' : '\"'))
    {
        buffer[buffer_index++] = input[*i];
        (*i)++;
    }
    if (input[*i] == (quote_type == 1 ? '\'' : '\"'))
    {
        buffer[buffer_index] = '\0';
        if (buffer_index > 0)
            append_token(tokens, buffer, quote_type == 1 ? SINGLE_Q : DOUBLE_Q, quote_type);
        (*i)++;
    }
}

void handle_redirections_and_pipes(char *input, int *i, t_token **tokens)
{
    if (input[*i] == '|')
    {
        append_token(tokens, "|", PIPE, 0);
        (*i)++;
    }
    else if (input[*i] == '<')
    {
        if (input[*i + 1] == '<')
        {
            append_token(tokens, "<<", HEREDOC, 0);
            (*i) += 2;
        }
        else
        {
            append_token(tokens, "<", RED_IN, 0);
            (*i)++;
        }
    }
    else if (input[*i] == '>')
    {
        if (input[*i + 1] == '>')
        { // Append
            append_token(tokens, ">>", APPEND, 0);
            (*i) += 2; // Move past both characters
        }
        else
        {
            append_token(tokens, ">", RED_OUT, 0);
            (*i)++;
        }
    }
}

void handle_word(char *input, int *i, t_token **tokens)
{
    char buffer[256]; // Temporary buffer for token storage
    int buffer_index;

    buffer_index = 0;
    while (input[*i] && !isspace(input[*i]) && \
           input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
    {
        buffer[buffer_index++] = input[*i];
        (*i)++;
    }

    if (buffer_index > 0)
    {
        buffer[buffer_index] = '\0';
        append_token(tokens, buffer, classify_token(buffer), 0);
    }
}

t_token *tokenize(char *input)
{
    t_token *tokens;
    int     i;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        while (ft_isspace(input[i]))
            i++;
        if (input[i] == '\'' || input[i] == '\"')
        {
            handle_quotes(input, &i, &tokens);
            continue ;
        }
        if (input[i] == '|' || input[i] == '<' || input[i] == '>') {
            handle_redirections_and_pipes(input, &i, &tokens);
            continue ;
        }
        handle_word(input, &i, &tokens);
    }
    return (tokens);
}
