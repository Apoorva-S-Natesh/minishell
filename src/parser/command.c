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
void set_cmd_priorities(t_command *cmd_head)
{
    int priority = 0;
    t_command *cmd = cmd_head;

    // First pass to count how many heredoc commands (priority == 0) exist
    while (cmd != NULL)
    {
        if (cmd->priority == 0)
            priority++;  // Count heredoc commands to start non-heredoc priorities later
        cmd = cmd->next;
    }

    // Second pass to assign increasing priority for commands without heredoc
    cmd = cmd_head;
    while (cmd != NULL)
    {
        if (cmd->priority != 0) {  // Skip commands with heredoc priority
            cmd->priority = priority;
            priority++;
        }
        cmd = cmd->next;  // Move to the next command
    }
}
