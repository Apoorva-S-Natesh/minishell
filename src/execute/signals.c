/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:51:39 by asomanah          #+#    #+#             */
/*   Updated: 2024/10/03 12:58:37 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Handle Ctrl+C (SIGINT)
void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		//display a new prompt on a new line
		write(1, "\n", 1);// move to a new line
		rl_on_new_line();// Tell readline we are starting a new line
		rl_replace_line("", 0);// Clear the buffer
		rl_redisplay();//Redisplay the prompt
	}
}
