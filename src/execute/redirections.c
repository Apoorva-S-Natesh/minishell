/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:28:10 by asomanah          #+#    #+#             */
/*   Updated: 2024/11/01 13:27:50 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_err_msg(const char *filename, const char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	setup_input_redir(t_process *prcs, t_redirection *redir, t_shell *mini)
{
	int	result;

	close (prcs->input_fd);
	if (redir->type == 1)
		prcs->input_fd = open(redir->input_file, O_RDONLY);
	else if (redir->type == 4)
	{
		result = handle_heredoc(redir->input_file, mini);
		if (result == -2 || result == -1) // SIGINT received during heredoc
			return (result);
		else 
			prcs->input_fd = result;
	}
	if (prcs->input_fd < 0)
	{
		print_err_msg(redir->input_file, "No such file or directory");
		mini->last_exit_status = 1;
		return (0);
	}
	return (1);
}

int	setup_output_redir(t_process *prcs, t_redirection *redir, t_shell *mini)
{
	close (prcs->output_fd);
	if (redir->type == 2)
		prcs->output_fd = open(redir->output_file, O_WRONLY | \
		O_CREAT | O_TRUNC, 0644);
	else
		prcs->output_fd = open(redir->output_file, O_WRONLY | \
		O_CREAT | O_APPEND, 0644);
	if (prcs->output_fd < 0)
	{
		print_err_msg(redir->output_file, "Permission denied");
		mini->last_exit_status = 1;
		return (0);
	}
	return (1);
}

int	setup_redirs(t_command *cmd, t_process *prcs, \
	t_redir_info *re, t_shell *mini)
{
	t_redirection	*redir;
	int				result;

	redir = cmd->redirection;
	prcs->input_fd = dup(re->tempin);
	prcs->output_fd = dup(re->tempout);
	while (redir)
	{
		if (redir->type == 1 || redir->type == 4)
		{
			result = setup_input_redir (prcs, redir, mini);
			if (result == -2 || result == 0)
				return (result);
		}
		else if (redir->type == 2 || redir->type == 3)
		{
			if (!setup_output_redir (prcs, redir, mini))
				return (-1);
		}
		redir = redir->next;
	}
	dup2(prcs->input_fd, 0);
	dup2(prcs->output_fd, 1);
	return (1);
}

void	cleanup_redirections(t_process *prcs)
{
	if (prcs->input_fd > 2)
		close(prcs->input_fd);
	if (prcs->output_fd > 2)
		close(prcs->output_fd);
}
