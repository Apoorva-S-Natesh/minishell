/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:24:35 by asomanah          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/10/31 17:10:45 by asomanah         ###   ########.fr       */
=======
/*   Updated: 2024/10/31 13:35:33 by asomanah         ###   ########.fr       */
>>>>>>> refs/remotes/origin/execute_pipe
/*                                                                            */
/* ************************************************************************** */

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
	if (prcs->signal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	else if (prcs->signal == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	free(prcs->cmd_path);
	prcs->cmd_path = NULL;
}

void	handle_child_process(t_exec_info *exec_info)
{
	setup_child_signals();
<<<<<<< HEAD
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{// Redirect stdin to the read end of the previous pipe
=======
	setup_child_pipes(exec_info);
	execute_command(exec_info->cmd, exec_info->prcs, exec_info->mini);
	exit (exec_info->mini->last_exit_status);
}

void	setup_child_pipes(t_exec_info *exec_info)
{
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{	// Redirect stdin to the read end of the previous pipe
>>>>>>> refs/remotes/origin/execute_pipe
		if (dup2(exec_info->pipe_info.prev_pipe[0], STDIN_FILENO) == -1)
			perror("dup2");
		close(exec_info->pipe_info.prev_pipe[0]);
		close(exec_info->pipe_info.prev_pipe[1]);
	}
	if (exec_info->cmd->next != NULL)
<<<<<<< HEAD
	{// Redirect stdout to the write end of the current pipe
		close(exec_info->pipe_info.pipe_fd[0]);
=======
	{
		close(exec_info->pipe_info.pipe_fd[0]);
		// Redirect stdout to the write end of the current pipe
>>>>>>> refs/remotes/origin/execute_pipe
		if (dup2(exec_info->pipe_info.pipe_fd[1], STDOUT_FILENO) == -1)
			perror ("dup2");
		close(exec_info->pipe_info.pipe_fd[1]);
	}
<<<<<<< HEAD
	// if (exec_info->mini->signal_pipe[0] != -1)
	// 	close(exec_info->mini->signal_pipe[0]);
	// if (exec_info->mini->signal_pipe[1] != -1)
	// 	close(exec_info->mini->signal_pipe[1]);
	close(exec_info->mini->signal_pipe[0]);
	close(exec_info->mini->signal_pipe[1]);
	execute_command(exec_info->cmd, exec_info->prcs, exec_info->mini);
	exit(exec_info->mini->last_exit_status);
=======
	close(exec_info->mini->signal_pipe[0]);
	close(exec_info->mini->signal_pipe[1]);
>>>>>>> refs/remotes/origin/execute_pipe
}

void	wait_for_child(t_process *prcs, t_shell *mini)
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
