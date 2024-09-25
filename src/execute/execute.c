#include "minishell.h"

void	execute(t_shell *mini)
{
	t_process	prcs;
	t_command	*cmd;

	cmd = mini->commands;

	if (cmd->redirection->input_file)
		prcs.input_fd = open(cmd->redirection->input_file, O_RDONLY);
	else
		prcs.input_fd = STDIN_FILENO;
	while(cmd != NULL)
	{
		dup2(prcs.input_fd, 0); //Redirect input to come from input_fd could be file or a pipe
		if(is_builtin(cmd->tokens[0])) //Check if the command is a built in
			execute_builtin(cmd, mini);
		expand_env(&mini); //Expand environment variables in each command token
		handle_redirection(cmd->redirection); //Handle redirection (input/output/append/heredoc)
		execute_external(cmd, &prcs);
		cmd = cmd->next;
	}
}
void	execute_external(t_command *cmd, t_process *prcs)
{
	if (cmd->next != NULL)
	{
		if(pipe(prcs->pipe_fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
	}
	prcs->pid = fork(); //Fork a new process to run the command
	if (prcs->pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (prcs->pid == 0)
	{
		child_process(cmd, &prcs); //child process setup
		execve(cmd->tokens[0], cmd->tokens, NULL); // send env variable here
		perror("execve");
		exit(1);
	}
	else
		parent_process(&prcs);
}
void	parent_process(t_process *prcs)
{
	waitpid(prcs->pid, NULL, 0); // Wait for child process to finish
	if (prcs->input_fd != STDIN_FILENO)
		close(prcs->input_fd);
	if (prcs->pipe_fd != NULL)
	{
		close(prcs->pipe_fd[1]); // Close the write end of the pipe
		prcs->input_fd = prcs->pipe_fd[0]; // Set the input for the next command to the read end of the pipe
	}
}