/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:24:35 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 21:01:17 by asomanah         ###   ########.fr       */
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

int	handle_child_process(t_exec_info *exec_info)
{
	int	err_no;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{
		if (dup2(exec_info->pipe_info.prev_pipe[0], STDIN_FILENO) == -1)
			perror("dup2");
		close(exec_info->pipe_info.prev_pipe[0]);
	}
	if (exec_info->pipe_info.prev_pipe[1] != -1)
		close(exec_info->pipe_info.prev_pipe[1]);
	if (exec_info->cmd->next != NULL && exec_info->pipe_info.pipe_fd[1] != -1)
	{
		if (dup2(exec_info->pipe_info.pipe_fd[1], STDOUT_FILENO) == -1)
			perror ("dup2");
	}
	if (exec_info->pipe_info.pipe_fd[0] != -1)
		close(exec_info->pipe_info.pipe_fd[0]);
	if (exec_info->pipe_info.pipe_fd[0] != -1)
		close(exec_info->pipe_info.pipe_fd[1]);
	close(exec_info->mini->signal_pipe[0]);
	close(exec_info->mini->signal_pipe[1]);
	err_no = execute_command(exec_info->cmd, exec_info->prcs, exec_info->mini);
	return (err_no);
}

void	wait_for_child(t_process *prcs)
{
	int		status;
	pid_t	wpid;

	wpid = waitpid(prcs->pid, &status, WUNTRACED);
	while (wpid > 0)
	{
		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			prcs->status = status;
			break ;
		}
	}
	restore_main_signals();
}

void	execute_non_builtin(t_command *cmd, t_process *prcs, \
t_shell *mini, t_pipe_info *pipe_info)
{
	t_exec_info	exec_info;
	int			exit_status;

	exec_info.cmd = cmd;
	exec_info.prcs = prcs;
	exec_info.mini = mini;
	exec_info.pipe_info = *pipe_info;
	prcs->pid = fork();
	if (prcs->pid == 0)
	{
		exit_status = handle_child_process(&exec_info);
		exit(exit_status);
	}
	else if (prcs->pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
		handle_parent_process(&exec_info);
}
