/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:27 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 16:57:53 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	initialize_process(t_process *prcs)
{ // Setting everything to invalid values
	prcs->pid = -1; //means no process has been created yet
	prcs->input_fd = -1; // means no input redirecrtion has been set yet
	prcs->output_fd = -1;
	prcs->pipe_fd[0] = -1; // Means no pipe has been created yet
	prcs->pipe_fd[1] = -1;
	prcs->status = 0; //later used to store the exit status of the child process.
	prcs->exit_code = 0; 
	prcs->signal = 0;
	prcs->cmd_path = NULL; //This indicates that no command path has been found or allocated yet
}

void	setup_redirs(t_command *cmd, t_process *prcs, int tempin, int tempout)
{
	t_redirection	*redir;

	redir = cmd->redirection;
	prcs->input_fd = dup(tempin);
	prcs->output_fd = dup(tempout);
	while (redir)
	{
		if (redir->type == 1 || redir->type == 4)
		{
			close (prcs->input_fd);
			if (redir->type == 1)
				prcs->input_fd = open(redir->input_file, O_RDONLY);
			else
				prcs->input_fd = handle_heredoc(redir->input_file);
			if (prcs->input_fd < 0)
			{
				perror("open");
				return ;
			}
		}
		else if (redir->type == 2 || redir->type == 3)
		{
			clsoe (prcs->output_fd);
			if (redir->type == 2)
				prcs->output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				prcs->output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (prcs->output_fd < 0)
			{
				perror ("open");
				return ;
			}
		}
		redir = redir->next;
	}
	dup2(prcs->input_fd, 0);
	dup2(prcs->output_fd, 1);
}

void	execute(t_shell *mini)
{
	t_process	prcs;
	t_command	*cmd;
	int			tempin;
	int			tempout;

	tempin = dup(0); // Save the current input
	tempout = dup(1); // Save the current output
	cmd = mini->commands;
	initialize_process(&prcs);

// Setup initial input
	//prcs.input_fd = set_initial_input(cmd, &prcs, tempin);
	//if (prcs.input_fd < 0)
	//	return ;
// Running through command
	while(cmd != NULL)
	{
		//Redirect Input
		//dup2(prcs.input_fd, 0); //Redirect input to come from input_fd could be file or a pipe
		//close (prcs.input_fd);
		setup_redirs(cmd, &prcs, tempin, tempout);
		if (prcs.input_fd < 0 || prcs.output_fd < 0)
			return ;
// Setup the Output
		//if (cmd->next == NULL) //Last command
		//	prcs.output_fd = set_output(cmd, &prcs, tempout);
		//if (prcs.output_fd < 0)
		//	return ;
		//else //not last command
		if (cmd->next != NULL)
		{
			if(pipe(prcs.pipe_fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
			prcs.output_fd = prcs.pipe_fd[1]; //Write to the pipe
			//prcs.input_fd = prcs.pipe_fd[0]; //Read from the pipe
		}
		//redirect output to output_fd(could be file or pipe)
		//dup2(prcs.output_fd, 1);
		//close(prcs.output_fd);
		//execute command
		execute_command(cmd, &prcs, mini);
		cleanup_redirections(&prcs);
		if (cmd->next != NULL)
			prcs.input_fd = prcs.pipe_fd[0];
		cmd = cmd->next; //Move to next command
	}
	//Restore the original input/output
	dup2(tempin, 0);
	dup2(tempout, 1);
	close(tempin);
	close(tempout);
	while (wait(&prcs.status) > 0); // Waiting for the last command to finish
}

void	cleanup_redirections(t_process *prcs)
{
	if (prcs->input_fd > 2)
		close(prcs->input_fd);
	if (prcs->output_fd > 2)
		close(prcs->output_fd);
}

void	execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	if(is_builtin(cmd->tokens[0])) //Check if the command is a built-in
		handle_builtin(cmd, mini);
	else // Fork and execute command
	{
		prcs->cmd_path = find_command(cmd->tokens[0], mini->env);
		if (!prcs->cmd_path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(cmd->tokens[0], 2);
			ft_putstr_fd("\n", 2);
			mini->last_exit_status = 127; //Could not find exit status
			return ;
		}
		prcs->pid = fork();
		if (prcs->pid == 0)
		{
			execve(prcs->cmd_path, cmd->tokens, mini->env); // send env variable here
			perror("execve");//If execve fails, print an error and exit
			exit(1);
		}
		else if (prcs->pid < 0) // If fork fails, print error and exit the shell
		{
			perror("fork");
			free(prcs->cmd_path);
			exit(1);
		}
		else //Parent process wait for the child to complete
		{
			waitpid(prcs->pid, &prcs->status, 0);
			handle_child_status(prcs, mini);
		}
	}
}

void	handle_child_status(t_process *prcs, t_shell *mini)
{
	if (WIFEXITED(prcs->status)) //Checkinig how the child exited
	{
		prcs->exit_code = WEXITSTATUS(prcs->status); //If not 0 then process has issue
		mini->last_exit_status = prcs->exit_code; // Update last exit status
		printf("Command exited with the code %d\n", prcs->exit_code); // Delete later
	}	
	else if (WIFSIGNALED(prcs->status)) //Checking if a signal stopped the child process suddenly
	{
		prcs->signal = WTERMSIG(prcs->status);
		mini->last_exit_status = 128 + prcs->signal;
		printf("COmmand termniated by signal %d\n", prcs->signal);
	}
	free(prcs->cmd_path);
	prcs->cmd_path = NULL;
}

/*
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
*/