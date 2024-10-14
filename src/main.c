/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/25 09:54:34 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void print_command_tokens(t_command *cmd);
static void print_redirections(t_redirection *redir);

static void print_tokens(t_token *tokens) {
    t_token *current = tokens;

    while (current != NULL) {
        // Print the value of the token
        printf("Value: %s, ", current->value);

        // Print the quote status of the token
        if (current->quote == 0) {
            printf("Quote: None\n");
        } else if (current->quote == 1) {
            printf("Quote: Single Quote\n");
        } else if (current->quote == 2) {
            printf("Quote: Double Quote\n");
        } else {
            printf("Quote: Invalid\n");
        }

        // Move to the next token
        current = current->next;
    }
}

static void print_commands(t_command *commands)
{
    t_command *current_command = commands;
    int cmd_count = 1;

    while (current_command)
    {
        printf("Command %d:\n", cmd_count);

        // Print tokens (arguments and command)
        print_command_tokens(current_command);

        // Print redirection information
        if (current_command->redirection)
            print_redirections(current_command->redirection);

        // Proceed to next command in the list (in case of pipes)
        current_command = current_command->next;
        cmd_count++;
        printf("\n"); // Separate commands with a newline
    }
}

static void print_command_tokens(t_command *cmd)
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

static void print_redirections(t_redirection *redir)
{
    t_redirection *current_redir = redir;

    printf("  Redirections:\n");
    while (current_redir)
    {
        if (current_redir->input_file)
            printf("    Input Redirect: <%s>\n", current_redir->input_file);
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

int	main(int ac, char **av, char **envv)
{
	t_shell	mini;
	t_token	*tokens;

	if (!check_args(ac, av))
		return (0);
	init_shell(&mini, envv);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	// Note: Ctrl-D is handled by readline, not as a signal
	while (mini.running_status)
	{
		if (take_input(&mini))
		{
			tokens = tokenize(&mini);
			if (!tokens)
				continue ;
			expand_tokens(tokens, mini.env);
			print_tokens(tokens);
			mini.commands = group_tokens_to_cmd(tokens);
			set_cmd_priorities(mini.commands);
			print_commands(mini.commands);
			execute(&mini);
		}
		free_tokens(tokens);
		free_command(mini.commands);
	}
	free_list(mini.env);
	return (0);
}

/*
Ctrl-D is not actually a signal, but is handled by the readline library as an EOF condition.
You should handle this in your take_input function. If readline returns NULL, it
means EOF was encountered (Ctrl-D was pressed).*/
