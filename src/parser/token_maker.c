/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:30:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/14 14:13:23 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token *init_empty_token(void)
{
    t_token *new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (NULL);
    new_token->value = NULL;           // Initialize value to NULL
    new_token->type = 0;               // Assuming 0 is a default/invalid type
    new_token->quote = 0;              // No quotes by default
    new_token->concat_flag = 0;        // No concatenation by default
    new_token->next = NULL;            // Initialize next to NULL

    return new_token;
}

void append_token(t_token **tokens, char *value, e_token_type type, int quote_type)
{
    t_token *new;
    t_token *current;

   /* if (ft_strlen(value) == 0)
        return;*/
    new = init_empty_token();
    if (!new)
        return;
    new->value = ft_strdup(value);
    if (!new->value)
    {
        free(new); // Free the token if value allocation fails
        return;
    }
    new->type = type;
    new->quote = quote_type;
    if (!*tokens)
        *tokens = new;
    else
    {
        current = *tokens;
        while (current->next)
            current = current->next;
        current->next = new;
	}
}

e_token_type	classify_token(char *token_value)
{
	if (ft_strcmp(token_value, "<") == 0)
		return (RED_IN);
	if (ft_strcmp(token_value, "<<") == 0)
		return (HEREDOC);
	if (ft_strcmp(token_value, ">") == 0)
		return (RED_OUT);
	if (ft_strcmp(token_value, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(token_value, "|") == 0)
		return (PIPE);
	return (WORD);
}

void set_concat_flag(char *input, int i, t_token *last_token)
{
	if (!last_token)
		return ;
	if (input[i] && !ft_isspace(input[i]) && input[i] != '|' && input[i] != '<' && input[i] != '>')
	{
		last_token->concat_flag = 1;
	}
}

void	append_or_concat_token(t_token **tokens, char *value, int type, int quote_type)
{
	t_token	*last_token;
	char	*new_value;

	last_token = get_last_token(*tokens);
	if (last_token != NULL)
	{
		if (last_token->concat_flag == 1)
		{
			new_value = ft_strjoin(last_token->value, value);
			if (new_value)
			{
				free(last_token->value);
				last_token->value = new_value;
				last_token->concat_flag = 0;
			}
		}
		else
			append_token(tokens, value, type, quote_type);
	}
	else
		append_token(tokens, value, type, quote_type);
}

t_token *get_last_token(t_token *tokens)
{
	if (tokens == NULL)
		return (NULL);
	while (tokens->next != NULL)
		tokens = tokens->next;
	return (tokens);
}
