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

void	handle_sigquit(int signum)
{
	(void)signum; //// Do nothing for SIGQUIT, we'll handle it in the main loop
}

void	setup_sig_handling(t_shell *mini)
{
	struct	sigaction sa_int;
	struct	sigaction sa_quit;
	
	if (pipe(mini->signal_pipe) == -1)//create a signal pipe
	{
		perror("pipe");
		exit (1);
	}
	// Set up SIGINT handler
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Set up SIGQUIT handler to ignore in the main process
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}
