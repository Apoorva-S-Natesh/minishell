/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:27 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/29 14:27:30 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_parent_process(int prev_pipe[2], int pipe_fd[2], t_command *cmd)
{
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
	else //close the pipe file descriptors if it's the last command.
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

static void	wait_for_child(t_process *prcs, t_shell *mini)
{
	fd_set			readfds;
	struct timeval	tv;
	int				ret;
	char			sig;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(mini->signal_pipe[0], &readfds);
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		ret = select(mini->signal_pipe[0] + 1, &readfds, NULL, NULL, &tv);
		if (ret == -1 && errno != EINTR)
		{
			perror("select");
			break ;
		}
		else if (ret > 0 && (read(mini->signal_pipe[0], &sig, 1)) > 0 && (sig == SIGINT))
		{
			kill(prcs->pid, SIGINT);
			write(STDOUT_FILENO, "\n", 1);
		}
		if (waitpid(prcs->pid, &prcs->status, WNOHANG) != 0)
			break ;
	}
}

static void	execute_non_builtin(t_command *cmd, t_process *prcs, \
t_shell *mini, int prev_pipe[2])
{
	int	pipe_fd[2];

	if (setup_pipes(pipe_fd, cmd) != 0)
	{
		mini->last_exit_status = 1;
		return ;
	}
	prcs->pid = fork();
	if (prcs->pid == 0)
	{
		handle_child_process(prev_pipe, pipe_fd, cmd, mini);
		execute_command(cmd, prcs, mini);
		exit(mini->last_exit_status);
	}
	else if (prcs->pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else
	{
		mini->foreground_pid = prcs->pid;
		handle_parent_process(prev_pipe, pipe_fd, cmd);
	}
}

void	execute_single_command(t_command *cmd, t_process *prcs, \
t_shell *mini, int prev_pipe[2])
{
	int	stdout_copy;
	int	pipe_fd[2];

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	setup_redirs(cmd, prcs, &mini->redir_info, mini);
	if (!cmd->tokens || !cmd->tokens[0]) // Handle the case where there's only a redirection (e.g., heredoc)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return ;
			}
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = pipe_fd[1];
		}
		return ;
	}
	if (cmd->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return ;
		}
	}
	if (is_builtin(cmd)) // when there's a pipe. We create a pipe, redirect stdout to the pipe, execute the built-in, and then restore stdout.
	{
		stdout_copy = dup(STDOUT_FILENO);
		if (cmd->next)
		{
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			prev_pipe[0] = pipe_fd[0];
			prev_pipe[1] = -1;
		}
		handle_builtin(cmd, mini);
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdout_copy);
	}
	else
	{
		execute_non_builtin(cmd, prcs, mini, prev_pipe);
		if (cmd->next == NULL)
		{
			wait_for_child(prcs, mini);
			mini->foreground_pid = -1;
			handle_child_status(prcs, mini);
		}
	}
}

void	execute(t_shell *mini)
{
	t_process	prcs;
	t_command	*cmd;
	int prev_pipe[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	mini->redir_info.tempin = dup(STDIN_FILENO);
	mini->redir_info.tempout = dup(STDOUT_FILENO);
	cmd = mini->commands;
	initialize_process(&prcs);
	while (cmd != NULL)
	{
		execute_single_command(cmd, &prcs, mini, prev_pipe);
		cleanup_redirections(&prcs);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0);
	dup2(mini->redir_info.tempin, STDIN_FILENO);
	dup2(mini->redir_info.tempout, STDOUT_FILENO);
	close(mini->redir_info.tempin);
	close(mini->redir_info.tempout);
}

void	execute_command(t_command *cmd, t_process *prcs, t_shell *mini)
{
	char	**env_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_array = create_env_array(mini->env);
	prcs->cmd_path = find_command(cmd->tokens[0], mini->env);
	printf("Debug : path is : %s\n", prcs->cmd_path);
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
