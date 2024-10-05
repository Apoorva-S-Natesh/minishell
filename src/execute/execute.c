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

void	execute(t_shell *mini)
{
	t_process	prcs;
	t_command	*cmd;
	int			tempin;
	int			tempout;

	tempin = dup(0); // Save the current input
	tempout = dup(1); // Save the current output
	cmd = mini->commands;
	prcs.cmd_path = NULL; // *******Need to initialize other variables of the prcs stuct???  

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
		else //Parent process wait for the chile to complete
		{
			waitpid(prcs->pid, &prcs->status, 0);
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
				free(prcs->cmd_path);
				prcs->cmd_path = NULL;
			}
		}
	}
}

//Need to handle multiple redirections
/*
Thank you for providing the detailed structure information. Based on this, I can provide a more tailored solution for handling multiple redirections in a single command. Here's an updated execution logic that should handle complex commands with multiple redirections:

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void execute_command(t_command *cmd, t_shell *mini)
{
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    t_redirection *redir = cmd->redirection;
    int input_fd = -1;
    int output_fd = -1;

    // Handle all redirections
    while (redir)
    {
        if (redir->type == 1 || redir->type == 4) // Input redirection or heredoc
        {
            if (input_fd != -1)
                close(input_fd);
            if (redir->type == 1)
                input_fd = open(redir->input_file, O_RDONLY);
            else
                input_fd = handle_heredoc(redir->input_file); // Implement this function
            if (input_fd == -1)
            {
                perror("open");
                return;
            }
        }
        else if (redir->type == 2 || redir->type == 3) // Output redirection
        {
            if (output_fd != -1)
                close(output_fd);
            if (redir->type == 2)
                output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else
                output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd == -1)
            {
                perror("open");
                return;
            }
        }
        redir = redir->next;
    }

    // Apply redirections
    if (input_fd != -1)
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != -1)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    // Execute the command
    if (is_builtin(cmd->tokens[0]))
    {
        handle_builtin(cmd, mini);
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            char *cmd_path = find_command(cmd->tokens[0], mini->env);
            if (!cmd_path)
            {
                fprintf(stderr, "minishell: command not found: %s\n", cmd->tokens[0]);
                exit(127);
            }
            execve(cmd_path, cmd->tokens, mini->env);
            perror("execve");
            exit(1);
        }
        else if (pid < 0)
        {
            perror("fork");
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                mini->last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                mini->last_exit_status = 128 + WTERMSIG(status);
        }
    }

    // Restore original stdin and stdout
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}
```

This implementation:

1. Handles multiple redirections of different types in a single command.
2. Processes redirections in the order they appear in the command, which is important for commands like `echo hello > file1 < input.txt >> file2`.
3. Correctly handles input and output redirections, including heredocs (though you'll need to implement the `handle_heredoc` function).
4. Restores the original stdin and stdout after command execution.
5. Works for both built-in commands and external commands.

To use this in your shell:

1. Call this `execute_command` function for each command in your command list.
2. Implement the `handle_heredoc` function to deal with heredoc input.
3. Ensure your `find_command` function works as expected to locate the full path of commands.
4. Make sure your `is_builtin` and `handle_builtin` functions are properly implemented.

This implementation should correctly handle complex commands with multiple redirections, such as `echo hello > file1 < input.txt >> file2`. It processes each redirection in order, which means:

1. `> file1` is set up first (but will be overwritten by the later `>> file2`)
2. `< input.txt` is then set as the input
3. Finally, `>> file2` is set as the output, appending to file2

The command will then execute with input from `input.txt` and output appended to `file2`.
*/