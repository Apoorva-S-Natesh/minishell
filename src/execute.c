#inlcude "minishell.h"

void	execute(t_shell *mini)
{
	int	in_fd;
	int	pipe_fd[2];
	pid_t	pid;

	in_fd = STDIN_FILENO;
	while(mini->commands != NULL)
	{
		check_builtin(mini->commands.tokens[0]); //Check if the command is a built in
		expand_env(&mini); //Expand environment variables in each command token
		handle_redirection(mini->commands.redirection); //Handle redirection (input/output/append/heredoc)
		if (mini->commands.next != NULL)
			pipe(pipe_fd);
		pid = fork(); //Fork a new process to run the command
		if (pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			child_process() //What should be the parameter for this?
		}
		else // Parent process
		{
			waitpid(pid, NULL, 0);
		}
		mini->commands = mini->commands->next;
	}
}
