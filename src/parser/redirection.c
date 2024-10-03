#include "../../includes/minishell.h"

void handle_redirection(t_command **cmd, t_token **current_token)
{
    t_token	*redir_token;
    t_token	*next_token;

	redir_token = *current_token;
	next_token = (*current_token)->next;
    if (next_token && next_token->type == WORD)
    {
        add_redi_to_cmd(*cmd, redir_token, next_token->value);
        *current_token = next_token;
    }
    else
    {
        printf("Syntax error: missing file after redirection\n");
    }
}

void add_redi_to_cmd(t_command *cmd, t_token *redir_token, char *filename)
{
    t_redirection *redir;

	redir = malloc(sizeof(t_redirection));
    if (!redir)
		return ;
	redir->input_file = NULL;
    redir->output_file = NULL;
    redir->type = 0;
    redir->next = NULL;
    set_redi_type(redir, redir_token, filename);
    append_redi(cmd, redir);
}

void set_redi_type(t_redirection *redir, t_token *redir_token, char *filename)
{
    if (redir_token->type == RED_IN)
	{
        redir->input_file = ft_strdup(filename);
		redir->type = 1;
	}
    else if (redir_token->type == RED_OUT)
	{
        redir->output_file = ft_strdup(filename);
		redir->type = 2;
	}
    else if (redir_token->type == APPEND)
    {
        redir->output_file = ft_strdup(filename);
        redir->type = 3;
    }
	else if (redir_token->type == HEREDOC)
    {
        redir->output_file = ft_strdup(filename);
        redir->type = 4;
    }
}


void append_redi(t_command *cmd, t_redirection *redir)
{
	t_redirection *temp;

    if (!cmd->redirection)
        cmd->redirection = redir;
    else
    {
        temp = cmd->redirection;
        while (temp->next)
            temp = temp->next;
        temp->next = redir;
    }
	if (redir->type == 4)
		cmd->priority = 0;
}

