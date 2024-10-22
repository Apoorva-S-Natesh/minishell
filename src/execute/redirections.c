#include "../../includes/minishell.h"

void	print_redir_err(const char *filename, const char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	setup_input_redir(t_process *prcs, t_redirection *redir, t_shell *mini)
{
	close (prcs->input_fd);
	if (redir->type == 1)
		prcs->input_fd = open(redir->input_file, O_RDONLY);
	else if (redir->type == 4)
		prcs->input_fd = handle_heredoc(redir->input_file, mini);
	if (prcs->input_fd < 0)
	{
		print_redir_err(redir->input_file, "No such file or directory");
		mini->last_exit_status = 1;
		return (0);
	}
	return (1);
}

int	setup_output_redir(t_process *prcs, t_redirection *redir, t_shell *mini)
{	
	close (prcs->output_fd);
	if (redir->type == 2)
		prcs->output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_TRUNC,
			 0644);
	else
		prcs->output_fd = open(redir->output_file, O_WRONLY | 
		O_CREAT | O_APPEND, 0644);
	if (prcs->output_fd < 0)
	{
		print_redir_err(redir->output_file, "Permission denied");
		mini->last_exit_status = 1;
		return (0);
	}
	return (1);
}

void	setup_redirs(t_command *cmd, t_process *prcs, \
	t_redir_info *re, t_shell *mini)
{
	t_redirection	*redir;

	redir = cmd->redirection;
	prcs->input_fd = dup(re->tempin);
	prcs->output_fd = dup(re->tempout);
	while (redir)
	{
		if (redir->type == 1 || redir->type == 4)
		{
			if(!setup_input_redir(prcs, redir, mini))
			return ;
		}
		else if (redir->type == 2 || redir->type == 3)
		{
			if(!setup_output_redir(prcs, redir, mini))
				return ;
		}
		redir = redir->next;
	}
	dup2(prcs->input_fd, 0);
	dup2(prcs->output_fd, 1);
}

void	cleanup_redirections(t_process *prcs)
{
	if (prcs->input_fd > 2)
		close(prcs->input_fd);
	if (prcs->output_fd > 2)
		close(prcs->output_fd);
}