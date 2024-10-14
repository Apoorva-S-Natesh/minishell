#include "../../includes/minishell.h"

/*The heredoc is typically handled in a child process to allow for proper signal 
handling and to prevent the main shell process from being affected by signals 
during heredoc input.*/


// Write a single line to the heredoc, expanding variables if necessary
static void	write_heredoc_line(int fd, char *line, t_shell *mini)
{
	char	*expanded_line;

	if (mini->expand_heredoc)
		expanded_line = expand_variables(line, mini); // Expand vairables in the line
	else
		expanded_line = expand_variables(line, mini); // Use the original line if expansion is disabled
	write(fd, expanded_line, ft_strlen(expanded_line)); // Write line to the file desciptor
	if (expanded_line != line)
		free(expanded_line);
	free(line); //free the origina line
}

//Main loop for reading the heredoc input
void	heredoc_read_loop(int fd, const char *delimiter, t_shell *mini)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2); //print hereodc promt
		line = get_next_line(STDIN_FILENO); //REad line from stdin
		if (!line)
		{
			write(STDERR_FILENO, HEREDOC_EOF, ft_strlen(HEREDOC_EOF)); // Handle EOF (CTRL+D)
			break ;
		}
		if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\n') //delim found, end of hererdoc
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line, mini); // Write the line to the heredoc
	}
}

//Child process function for handling heredoc input
void	heredoc_child_process(int wr_fd, const char *delimiter, t_shell *mini)
{
	if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(1);
	}
	heredoc_read_loop(wr_fd, delimiter, mini); //Read and process heredoc input
	close (wr_fd); //close the write end of the pipe
	exit (0); //Exit the child process
}

//Main function for handling heredoc
int	handle_heredoc(const char *delimiter, t_shell *mini)
{
	printf("Debug: handle heredoc entered, delimeter: %s \n", delimiter);//debug print
	if (!delimiter || !mini)
	{
			ft_putstr_fd("Error: Invalid arguments to handle_hd\n" ,STDERR_FILENO);
			return (-1);
	}
	t_process	hd_prcs;

	initialize_process(&hd_prcs);
	if (pipe(hd_prcs.pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	hd_prcs.pid = fork();
	if (hd_prcs.pid == -1)
	{
		perror("minishell: fork");
		close(hd_prcs.pipe_fd[0]);
		close(hd_prcs.pipe_fd[1]);
		return (-1);
	}
	else if (hd_prcs.pid == 0) //child process
	{
		close(hd_prcs.pipe_fd[0]); //closing read end of the pipe
		heredoc_child_process(hd_prcs.pipe_fd[1], delimiter, mini);
		exit (1);
	}
	else //Parent process
	{
		close(hd_prcs.pipe_fd[1]); //close write end of the pipe
		waitpid(hd_prcs.pid, &hd_prcs.status, 0);
		if (WIFSIGNALED(hd_prcs.status)) // Child process was terminated by a signal
		{
			close(hd_prcs.pipe_fd[0]);
			mini->last_exit_status = 128 + WTERMSIG(hd_prcs.status);
			return (-1);
		}
		return (hd_prcs.pipe_fd[0]); //Return the read end of the pipe
	}
	return (-1); //// This line should never be reached, but it satisfies the compiler
}
