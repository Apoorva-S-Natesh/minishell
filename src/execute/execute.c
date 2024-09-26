#include "minishell.h"

void	execute(t_shell *mini)
{
	t_process	prcs;
	t_command	*cmd;
	int			tempin;
	int			tempout;

	tempin = dup(0); // Save the current input
	tempout = dup(1); // Save the current output
	cmd = mini->commands;

// Setup initial input
	if (cmd->redirection && (cmd->redirection->input_file))
		prcs.input_fd = open(cmd->redirection->input_file, O_RDONLY);
		if (prcs.input_fd < 0)
		{
			perror ("open");
			return ;
		}
	else if (cmd->redirection && cmd->redirection->type == 4)
	{
		handle_heredoc(cmd->redirection->input_file);
		prcs.input_fd = dup(0);
	}
	else
		prcs.input_fd = dup(tempin);

// Running through command
	while(cmd != NULL)
	{
		//Redirect Input
		dup2(prcs.input_fd, 0); //Redirect input to come from input_fd could be file or a pipe
		close (prcs.input_fd);

		// Setup Output
		if (cmd->next == NULL) //Last command
		{
			if (cmd->redirection && cmd->redirection->output_file)
			{
				if (cmd->redirection->type == 2)
					prcs.output_fd = open(cmd->redirection->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else if (cmd->redirection->type == 3)
					prcs.output_fd = open(cmd->redirection->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (prcs.output_fd < 0)
				{
					perror("open");
					return ;
				}
			}
			else
				prcs.output_fd = dup(tempout);
		}
		else //not last command
		{
			if(pipe(prcs.pipe_fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
			prcs.output_fd = prcs.pipe_fd[1];
			prcs.input_fd = prcs.pipe_fd[0];
		}
		//redirect output
		dup2(prcs.output_fd, 1);
		close(prcs.output_fd);
		if(is_builtin(cmd->tokens[0])) //Check if the command is a built-in
			execute_builtin(cmd, mini);
		else // Fork and execute command
		{
			prcs.pid = fork();
			if (prcs.pid < 0)
			{
				perror("fork");
				exit(1);
			}
			else if (prcs.pid == 0)
			{
				execve(cmd->tokens[0], cmd->tokens, NULL); // send env variable here
				perror("execve");
				exit(1);
			}
			else
				waitpid(prcs.pid, prcs.status, 0);
		}
		cmd = cmd->next; //Move to next command
	}
	//Restore the original input/output
	dup2(tempin, 0);
	dup2(tempout, 1);
	close(tempin);
	close(tempout);
	while (wait(&prcs.status) > 0); // Waiting for the last command to finish
}

//expand_env(&mini); //Expand environment variables in each command token
//handle_redirection(cmd->redirection); //Handle redirection (input/output/append/heredoc)
		