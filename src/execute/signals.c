/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:39 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/01 12:53:34 by asomanah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Handle Ctrl+C (SIGINT)
void	handle_sigint(int signum)
{
	if (signum == SIGINT)
	{
		//display a new prompt on a new line
		write(1, "\n", 1);// move to a new line
		rl_on_new_line();// Tell readline we are starting a new line
		rl_replace_line("", 0);// Clear the buffer
		rl_redisplay();//Redisplay the prompt
	}
}

void	handle_sigint_heredoc(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	exit (1); //
}