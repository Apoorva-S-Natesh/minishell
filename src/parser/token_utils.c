/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:24:29 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/15 00:54:04 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*get_last_token(t_token *tokens)
{
	if (tokens == NULL)
		return (NULL);
	while (tokens->next != NULL)
		tokens = tokens->next;
	return (tokens);
}

// Function to handle quotes
void	handle_quote(t_tokenizer *tokenizer, int *i)
{
	if (!tokenizer->in_quotes)
	{
		tokenizer->in_quotes = 1;
		tokenizer->quote_char = tokenizer->input[*i];
	}
	else if (tokenizer->input[*i] == tokenizer->quote_char) 
	{
		tokenizer->in_quotes = 0;
		tokenizer->quote_char = 0;
	}
	tokenizer->buffer[tokenizer->buffer_index++] = tokenizer->input[*i];
	(*i)++;
}

// Main function to handle words
void	handle_word(t_shell *mini, int *i, t_token **tokens)
{
	t_tokenizer	tz;
	char		*cleaned_str;

	tz.input = mini->input;
	tz.buffer_index = 0;
	tz.in_quotes = 0;
	tz.quote_char = 0;
	while (tz.input[*i] && (!ft_isspace(tz.input[*i]) || tz.in_quotes))
	{
		if (tz.input[*i] == '\'' || tz.input[*i] == '\"')
			handle_quote(&tz, i);
		else if (tz.input[*i] == '=' && !tz.in_quotes)
			handle_equals(&tz, i);
		else if (should_break(tz.input[*i], tz.in_quotes))
			break ;
		else
			tz.buffer[tz.buffer_index++] = tz.input[(*i)++];
	}
	if (tz.buffer_index > 0)
	{
		tz.buffer[tz.buffer_index] = '\0';
		cleaned_str = remove_quotes(tz.buffer);
		append_or_concat_token(tokens, cleaned_str, WORD, 0);
		free(cleaned_str);
	}
}
