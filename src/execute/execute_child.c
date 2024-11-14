/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:24:35 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/14 15:35:33 by asomanah         ###   ########.fr       */
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

	// setup_child_signals();
	// Restore default signal handlers for the child process
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (exec_info->pipe_info.prev_pipe[0] != -1)
	{// Redirect stdin to the read end of the previous pipe
		if (dup2(exec_info->pipe_info.prev_pipe[0], STDIN_FILENO) == -1)
			perror("dup2");
		close(exec_info->pipe_info.prev_pipe[0]);
	}
	if (exec_info->pipe_info.prev_pipe[1] != -1)
		close(exec_info->pipe_info.prev_pipe[1]);
	if (exec_info->cmd->next != NULL && exec_info->pipe_info.pipe_fd[1] != -1)
	{// Redirect stdout to the write end of the current pipe
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

void wait_for_child(t_process *prcs)
{
    int status;
    pid_t wpid;

    while ((wpid = waitpid(prcs->pid, &status, WUNTRACED)) > 0)
    {
        if (WIFEXITED(status) || WIFSIGNALED(status))
        {
            prcs->status = status;
            break;
        }
    }

    // After the child process finishes, restore the custom SIGINT handler
    restore_main_signals();
}
