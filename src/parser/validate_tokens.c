/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:31:50 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 10:41:59 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_special_token(t_token *token)
{
	if (token->type == PIPE)
		return (1);
	else
		return (0);
}

static int	is_redirection(t_token *current)
{
	if ((current->type == RED_IN || current->type == RED_OUT || \
		current->type == APPEND))
		return (1);
	else
		return (0);
}

int	validate_tokens(t_token *tokens, t_shell *mini)
{
	t_token	*current;

	current = tokens;
	if (current && current->type == PIPE)
		return (-1);
	if (current && is_redirection(current))
		return (-3);
	while (current && current->next)
	{
		if (is_special_token(current) && is_special_token(current->next))
		{
			mini->last_exit_status = 2;
			return (-1);
		}
		current = current->next;
	}
	if (current && current->type == PIPE)
		return (-1);
	if (current && (current->type == RED_IN || current->type == RED_OUT || \
		current->type == APPEND || current->type == HEREDOC))
	{
		mini->last_exit_status = 2;
		return (-2);
	}
	return (1);
}
