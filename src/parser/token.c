/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:25:16 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/25 09:38:11 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	*start;
	char	*end;

	start = input;
	tokens = NULL;
	while (*start)
	{
		while (*start && ft_isspace(*start))
			start++;
		if (!*start)
			break;
		if (!create_tokens(&tokens, &start, &end))
			return (NULL);
		start = end;
	}
	return (tokens);
}

int	create_tokens(t_token **tokens, char **start, char **end)
{
	char			*token_start;
	char			*token_value;
	e_token_type	type;
	int				quote;

	token_start = *start;
	quote = 0;
	if (**start == '<' || **start == '>' || **start == '|')
		handle_operators(start, end);
	else if (**start == '\'' || **start == '\"')
	{
		if (!handle_quotes(start, end, &quote))
		{
			printf("unclosed quotes\n");
			return (0);
		}
	}
	else
		handle_words(start, end);
	token_value = ft_strndup(token_start, *end - token_start);
	type = classify_token(token_value);
	append_token(tokens, token_value, type, quote);
	free(token_value);
	return (1);
}

void	handle_operators(char **start, char **end)
{
	*end = *start + 1;
	if ((**start == '<' && **end == '<') || (**start == '>' && **end == '>') \
		|| (**start == '|' && **end == '|'))
	{
		(*end)++;
	}
	*start = *end;
}

int	handle_quotes(char **start, char **end, int *quote_type)
{
	char	quote;

	quote = **start;
	(*start)++;
	*end = *start;
	 if (quote == '\'')
		*quote_type = 1;  // Single quote
	else if (quote == '\"')
		*quote_type = 2;  // Double quote
	while (**end && **end != quote)
		(*end)++;
	if (!**end)
		return (0);
	(*end)++; // Include the closing quote
	*start = *end;
	return (1);
}

void handle_words(char **start, char **end)
{
	*end = *start;
	while (**end && !ft_isspace(**end) && **end != '<' && \
		**end != '>' && **end != '|' && **end != '\'' && **end != '\"')
	{
		(*end)++;
	}
	*start = *end;
}
