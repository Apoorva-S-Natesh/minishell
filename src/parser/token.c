/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:25:16 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/24 12:18:43 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*tokenize(char *input)
{
	t_token	tokens;
	char	*start;
	char	*end;

	start = input;
	while (*start)
	{
		while (*start && ft_isspace(*start))
			start++;
		if (!*start)
			break;
		end = start;
		if (*start == '<' || *start == '>' || *start == '|')
			start = handle_operators(start, &end);
		else if (*start == '\'' || *start == '\"')
		{

		}
		else
			while (*end && !ft_isspace(*end) && *end != '<' && *end != '>' && *end != '|')
				end++;
	}
}

char	*handle_operators(char *start, char **end)
{
	*end = start + 1;
    if ((*start == '<' && **end == '<') || (*start == '>' && **end == '>')\
		 || (*start == '|' && **end == '|'))
    	(*end)++;
    return (*end);
}
