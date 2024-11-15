/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:34:10 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 11:01:20 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*remove_quotes(char *str)
{
	char	*cleaned_str;
	char	*src;
	char	*dst;
	size_t	len;

	len = ft_strlen(str);
	cleaned_str = malloc(len + 1);
	if (!cleaned_str)
		return (NULL);
	src = str;
	dst = cleaned_str;
	while (*src)
	{
		if (*src != '\'' && *src != '\"')
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
	return (cleaned_str);
}

char	*extract_quoted_content(char *input, int *i, int quote_type, \
	int buffer_index)
{
	char	*buffer;
	char	closing_quote;

	buffer = malloc(256);
	if (buffer == NULL)
		return (NULL);
	(*i)++;
	if (quote_type == SINGLE_Q)
		closing_quote = '\'';
	else
		closing_quote = '\"';
	while (input[*i] && input[*i] != closing_quote)
	{
		buffer[buffer_index++] = input[*i];
		(*i)++;
	}
	if (input[*i] == closing_quote)
	{
		buffer[buffer_index] = '\0';
		(*i)++;
		return (buffer);
	}
	free(buffer);
	return (NULL);
}

// Function to handle expansion for double quotes
int	handle_double_quotes(t_shell *mini, char *quoted_content, \
	t_token **tokens, int *i)
{
	char	*expanded_content;

	if (ft_strlen(quoted_content) > 0)
	{
		expanded_content = expand_value(quoted_content, mini);
		if (!expanded_content)
			return (0);
		append_or_concat_token(tokens, expanded_content, DOUBLE_Q, 2);
		set_concat_flag(mini->input, *i, get_last_token(*tokens));
		free(expanded_content);
		free(quoted_content);
	}
	else 
	{
		append_or_concat_token(tokens, quoted_content, DOUBLE_Q, 2);
		set_concat_flag(mini->input, *i, get_last_token(*tokens));
		free(quoted_content);
	}
	return (1);
}

// Function to handle single quotes
int	handle_single_quotes(char *quoted_content, t_token **tokens, \
	int *i, t_shell *mini)
{
	append_or_concat_token(tokens, quoted_content, SINGLE_Q, 1);
	set_concat_flag(mini->input, *i, get_last_token(*tokens));
	free(quoted_content);
	return (1);
}

// Main function to handle quotes in the input
int	handle_quotes(t_shell *mini, int *i, t_token **tokens)
{
	int		quote_type;
	char	*quoted_content;
	int		buffer_index;

	buffer_index = 0;
	if (mini->input[*i] == '\'')
		quote_type = SINGLE_Q;
	else 
		quote_type = DOUBLE_Q;
	quoted_content = extract_quoted_content(mini->input, i, \
		quote_type, buffer_index);
	if (quoted_content == NULL)
	{
		printf("Error: Unclosed quote detected!\n");
		return (0);
	}
	if (quote_type == DOUBLE_Q)
		return (handle_double_quotes(mini, quoted_content, tokens, i));
	else
		return (handle_single_quotes(quoted_content, tokens, i, mini));
}
