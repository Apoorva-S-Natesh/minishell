/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:30:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/25 09:36:47 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void append_token(t_token **tokens, char *value, e_token_type type)
{
    t_token *new;
	t_token *current;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
    new->value = ft_strdup(value);
    new->type = type;
    new->next = NULL;

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

e_token_type classify_token(char *token_value)
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
