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
	prcs.input_fd = set_initial_input(cmd, &prcs, tempin);
	if (prcs.input_fd < 0)
		return ;
// Running through command
	while(cmd != NULL)
	{
		//Redirect Input
		dup2(prcs.input_fd, 0); //Redirect input to come from input_fd could be file or a pipe
		close (prcs.input_fd);

// Setup the Output
		if (cmd->next == NULL) //Last command
			prcs.output_fd = set_output(cmd, &prcs, tempout);
		if (prcs.output_fd < 0)
			return ;
		else //not last command
		{
			if(pipe(prcs.pipe_fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
			prcs.output_fd = prcs.pipe_fd[1]; //Write to the pipe
			prcs.input_fd = prcs.pipe_fd[0]; //Read from the pipe
		}
		//redirect output to output_fd(could be file or pipe)
		dup2(prcs.output_fd, 1);
		close(prcs.output_fd);
		//execute command
		execute_command(cmd, &prcs, mini);
		cmd = cmd->next; //Move to next command
	}
	//Restore the original input/output
	dup2(tempin, 0);
	dup2(tempout, 1);
	close(tempin);
	close(tempout);
	while (wait(&prcs.status) > 0); // Waiting for the last command to finish
}

// Setup initial input
int	set_initial_input(t_command	*cmd, t_process *prcs, int tempin)
{
	if (cmd->redirection && cmd->redirection->type == 4)
	{
		handle_heredoc(cmd->redirection->input_file);
		prcs->input_fd = dup(0);
	}
	else if (cmd->redirection && (cmd->redirection->input_file))
	{	
		prcs->input_fd = open(cmd->redirection->input_file, O_RDONLY);
		if (prcs->input_fd < 0)
		{
			perror ("open");
			return (-1);
		}
	}
	else
		prcs->input_fd = dup(tempin);
	return (prcs->input_fd);
}

int	set_output(t_command *cmd, t_process *prcs, int tempout)
{
	if (cmd->redirection && cmd->redirection->output_file)
	{
		if (cmd->redirection->type == 2)
			prcs->output_fd = open(cmd->redirection->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (cmd->redirection->type == 3)
			prcs->output_fd = open(cmd->redirection->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (prcs->output_fd < 0)
		{
			perror("open");
			return (-1);
		}
	}
	else
		prcs->output_fd = dup(tempout); // Send output to the screen
	return (prcs->output_fd);
}

void	execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	if(is_builtin(cmd->tokens[0])) //Check if the command is a built-in
		execute_builtin(cmd, mini);
	else // Fork and execute command
	{
		prcs->pid = fork();
		if (prcs->pid == 0)
		{
			execve(cmd->tokens[0], cmd->tokens, NULL); // send env variable here
			perror("execve");//If execve fails, print an error and exit
			exit(1);
		}
		else if (prcs->pid < 0) // If fork fails, print error and exit the shell
		{
			perror("fork");
			exit(1);
		}
		else //Parent process wait for the chile to complete
		{
			waitpid(prcs->pid, &prcs->status, 0);
			if (WIFEXITED(prcs->status)) //Checkinig how the child exited
			{
				prcs->exit_code = WEXITSTATUS(prcs->status); //If not 0 then process has issue
				printf("Command exited with the code %d\n", prcs->exit_code); // Delete later
			}	
			else if (WIFSIGNALED(prcs->status)) //Checking if a signal stopped the child process suddenly
			{
				prcs->signal = WTERMSIG(prcs->status);
				printf("COmmand termniated by signal %d\n", prcs->signal);
			}
		}
	}
}
