#include "../../includes/minishell.h"

// Write a single line to the heredoc, expanding variables if necessary
void	write_heredoc_line(int fd, char *line, t_shell *mini)
{
	char	*expanded_line;

	if (mini->expand_heredoc)
		expanded_line = expand_value(line, mini);
	else
		expanded_line = expand_value(line, mini);
	write(fd, expanded_line, ft_strlen(expanded_line));
	if (expanded_line != line)
		free(expanded_line);
	free(line);
}

//Main loop for reading the heredoc input
void	heredoc_read_loop(int fd, const char *delimiter, t_shell *mini)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			write(STDERR_FILENO, HEREDOC_EOF, ft_strlen(HEREDOC_EOF));
			break ;
		}
		if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\n')
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line, mini);
	}
}
