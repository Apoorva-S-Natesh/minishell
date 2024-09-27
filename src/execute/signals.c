#include "minishell.h"

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
