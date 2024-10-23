#include "../../includes/minishell.h"

void	handle_child_status(t_process *prcs, t_shell *mini)
{
	if (WIFEXITED(prcs->status))
	{
		prcs->exit_code = WEXITSTATUS(prcs->status);
		mini->last_exit_status = prcs->exit_code;
	}
	else if (WIFSIGNALED(prcs->status))
	{
		prcs->signal = WTERMSIG(prcs->status);
		mini->last_exit_status = 128 + prcs->signal;
	}
	free(prcs->cmd_path);
	prcs->cmd_path = NULL;
}

void	handle_child_process(int prev_pipe[2], int pipe_fd[2], \
	t_command *cmd, t_shell *mini)
{
	if (prev_pipe[0] != -1)
	{
		if(dup2(prev_pipe[0], STDIN_FILENO) == -1)
			perror("dup2");
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->next != NULL)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror ("dup2");
		close(pipe_fd[1]);
	}
	if (mini->signal_pipe[0] != -1) //Close all unnecessary file descriptors
		close(mini->signal_pipe[0]);
	if (mini->signal_pipe[1] != -1)
		close(mini->signal_pipe[1]);
	signal(SIGINT, SIG_DFL); //Reset signal handlers for the child process
	signal(SIGQUIT, SIG_DFL);
	//close(mini->signal_pipe[0]);
}
