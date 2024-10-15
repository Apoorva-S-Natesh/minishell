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
void	print_redir_err(const char *filename, const char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	setup_redirs(t_command *cmd, t_process *prcs, t_redir_info *re, t_shell *mini)
{
	t_redirection	*redir;

	redir = cmd->redirection;
	prcs->input_fd = dup(re->tempin);
	prcs->output_fd = dup(re->tempout);
	while (redir)
	{
		printf("Debug: Redirection type: %d\n", redir->type);//Debug print
		if (redir->type == 1 || redir->type == 4)
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
				return ;
			}
		}
		else if (redir->type == 2 || redir->type == 3)
		{
			close (prcs->output_fd);
			if (redir->type == 2)
				prcs->output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				prcs->output_fd = open(redir->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (prcs->output_fd < 0)
			{
				print_redir_err(redir->output_file, "Permission denied");
				mini->last_exit_status = 1;
				return ;
			}
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

char	**create_env_array(t_env *env)
{
	int		count;
	t_env	*temp;
	char	**env_array;
	int		i;
	char	*temp_str;

	count = 0;
	temp = env;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		temp_str = ft_strjoin(env->key, "=");
		if (!temp_str)
			return (NULL);
		env_array[i] = ft_strjoin(temp_str, env->value);
		free(temp_str);
		if (!env_array[i])
			return (NULL);
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

void execute(t_shell *mini)
{
    t_process prcs;
    t_command *cmd;
    t_redir_info redir_info;
    int prev_pipe[2] = {-1, -1};

    redir_info.tempin = dup(STDIN_FILENO);
    redir_info.tempout = dup(STDOUT_FILENO);
    cmd = mini->commands;
    initialize_process(&prcs);

    while (cmd != NULL)
    {
        int pipe_fd[2];
        if (cmd->next != NULL)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }
		setup_redirs(cmd, &prcs, &redir_info, mini);
		if (is_builtin(cmd))
    	{
        	handle_builtin(cmd, mini);
    	}
		else
		{
			prcs.pid = fork();
        	if (prcs.pid == 0)
        	{
            // Child process
            	if (prev_pipe[0] != -1)
            	{
                	dup2(prev_pipe[0], STDIN_FILENO);
                	close(prev_pipe[0]);
                	close(prev_pipe[1]);
            	}

            	if (cmd->next != NULL)
            	{
                	close(pipe_fd[0]);
                	dup2(pipe_fd[1], STDOUT_FILENO);
                	close(pipe_fd[1]);
            	}
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
				close(mini->signal_pipe[0]);  // Close read end of signal pipe
            	execute_command(cmd, &prcs, mini);
            	exit(mini->last_exit_status);
        	}
        	else if (prcs.pid < 0)
        	{
            	perror("fork");
            	exit(1);
        	}
        	else
        	{
            // Parent process
				mini->foreground_pid = prcs.pid;
            	if (prev_pipe[0] != -1)
            	{
                	close(prev_pipe[0]);
                	close(prev_pipe[1]);
            	}
            	if (cmd->next != NULL)
            	{
                	prev_pipe[0] = pipe_fd[0];
                	prev_pipe[1] = pipe_fd[1];
            	}
            	else
            	{
                	// Wait for child or signal
                    fd_set readfds;
                    while (1)
                    {
                        FD_ZERO(&readfds);
                        FD_SET(mini->signal_pipe[0], &readfds);

                        struct timeval tv = {0, 100000};  // 100ms timeout

                        int ret = select(mini->signal_pipe[0] + 1, &readfds, NULL, NULL, &tv);
                        if (ret == -1)
                        {
                            if (errno == EINTR) continue;  // Interrupted by a signal, try again
                            perror("select");
                            break;
                        }
                        else if (ret > 0)
                        {
                            // Signal received
                            char sig;
                            if (read(mini->signal_pipe[0], &sig, 1) > 0)
                            {
                                if (sig == SIGINT)
                                {
                                    kill(prcs.pid, SIGINT);
                                    write(STDOUT_FILENO, "\n", 1);
                                }
                                // else if (sig == SIGQUIT)
                                // {
                                //     kill(prcs.pid, SIGQUIT);
                                //     write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
                                // }
                            }
                        }

                        // Check if child has exited
                        if (waitpid(prcs.pid, &prcs.status, WNOHANG) != 0)
                        {
                            break;
                        }
                    }

                    mini->foreground_pid = -1;
                    handle_child_status(&prcs, mini);
            	}
			}
		}
		cleanup_redirections(&prcs);
        cmd = cmd->next;
    }

    // Wait for all remaining child processes
    while (wait(NULL) > 0);

    dup2(redir_info.tempin, STDIN_FILENO);
    dup2(redir_info.tempout, STDOUT_FILENO);
    close(redir_info.tempin);
    close(redir_info.tempout);
}

void execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	char **env_array;

	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    env_array = create_env_array(mini->env);
    prcs->cmd_path = find_command(cmd->tokens[0], mini->env);
    printf("path is : %s\n", prcs->cmd_path);
    if (!prcs->cmd_path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->tokens[0], 2);
		ft_putstr_fd("\n", 2);
		mini->last_exit_status = 127;
		free_env_array(env_array);
		exit(127);
	}
	execve(prcs->cmd_path, cmd->tokens, env_array);
	perror("execve");
	free_env_array(env_array);
	exit(1);
}
