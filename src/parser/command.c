/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:09:20 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 16:37:23 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*group_tokens_to_cmd(t_token *tokens)
{
	t_command	*head;
	t_command	*current_cmd;
	t_token		*current_tkn;

	head = NULL;
	current_cmd = NULL;
	current_tkn = tokens;
	while (current_tkn)
	{
		current_cmd = get_command(current_cmd, &head);
		process_token(&current_cmd, &current_tkn);
	}
	return (head);
}

t_command	*get_command(t_command *current, t_command **head)
{
	if (!current)
	{
		current = create_new_command();
		append_command(head, current);
	}
	return (current);
}

t_command	*create_new_command(void)
{
	t_command	*new_command;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->tokens = NULL;
	new_command->redirection = NULL;
	new_command->next = NULL;
	new_command->priority = -1;
	return (new_command);
}

void	append_command(t_command **head, t_command *new_command)
{
	t_command	*temp;

	if (!*head)
		*head = new_command;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_command;
	}
}

void	set_cmd_priorities(t_command *cmd_head)
{
	int			priority;
	t_command	*cmd;

	priority = 0;
	cmd = cmd_head;
	while (cmd != NULL)
	{
		if (cmd->priority == 0)
			priority++;
		cmd = cmd->next;
	}
	cmd = cmd_head;
	while (cmd != NULL)
	{
		if (cmd->priority != 0)
		{
			cmd->priority = priority;
			priority++;
		}
		cmd = cmd->next;
	}
}
