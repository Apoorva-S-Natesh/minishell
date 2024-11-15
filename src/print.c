/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:56:42 by aschmidt          #+#    #+#             */
/*   Updated: 2024/11/15 00:30:43 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tokens(t_token *tokens)
{
	t_token	*current = tokens;

	current = tokens;
	while (current != NULL)
	{
		printf("Value: %s, ", current->value);
		printf("Flag: %d, ", current->concat_flag);
		printf("Type: %d, ", current->type);
		if (current->quote == 0)
			printf("Quote: None\n");
		else if (current->quote == 1)
			printf("Quote: Single Quote\n");
		else if (current->quote == 2)
			printf("Quote: Double Quote\n");
		else
			printf("Quote: Invalid\n");
		current = current->next;
	}
}

void	print_commands(t_command *commands)
{
	t_command	*current_command = commands;;
	int			cmd_count;

	cmd_count = 1;
	while (current_command)
	{
		printf("Command %d:\n", cmd_count);
		print_command_tokens(current_command);
		if (current_command->redirection)
			print_redirections(current_command->redirection);
		current_command = current_command->next;
		cmd_count++;
		printf("\n");
	}
}

void	print_command_tokens(t_command *cmd)
{
	if (cmd->tokens)
	{
		printf("  Tokens:\n");
		for (int i = 0; cmd->tokens[i]; i++)
		{
			printf("    [%s]\n", cmd->tokens[i]);
		}
			printf("cmd priority %i\n", cmd->priority);
	}
	else
	{
		printf("  No tokens.\n");
	}
}

void print_redirections(t_redirection *redir)
{
    t_redirection *current_redir = redir;

    printf("  Redirections:\n");
    while (current_redir)
    {
        if (current_redir->input_file)
         {
            if (current_redir->type == 1)
                printf("    Input Redirect: >%s\n", current_redir->input_file);
            else if (current_redir->type == 4)
                printf("    Heredoc: <<%s\n", current_redir->input_file);
        }
        if (current_redir->output_file)
        {
            if (current_redir->type == 2)
                printf("    Output Redirect: >%s\n", current_redir->output_file);
            else if (current_redir->type == 3)
                printf("    Append Redirect: >>%s\n", current_redir->output_file);
			else if (current_redir->type == 4)
				printf("    Heredoc Redirect: <<%s\n", current_redir->output_file);
        }
        current_redir = current_redir->next;
    }
}
