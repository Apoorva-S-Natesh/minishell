
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int 	ft_free(char **rev_str);
int 	count_words(char *original);
int		reverse(char **rev_str, int count, char *ori);
void	print_str(char **rev_str);

int	main(int argc, char *argv[])
{
	int count = 0;
	char **rev_str;

	if (argc == 2)
	{
		count = count_words(argv[1]);
		rev_str = (char **)malloc(sizeof(char *) * (count + 1));
		if (!rev_str)
		{
			return(ft_free(rev_str));
		}
		rev_str[count] = NULL;
		if (reverse(rev_str, count, argv[1]) == -1)
			return(ft_free(rev_str));
		print_str(rev_str);
		ft_free(rev_str);
	}
	write(1, "\n", 1);
	return (0);
}

int count_words(char *original)
{
	int count = 0;
	int i = 0;
	while(original[i])
	{
		if ((original[i + 1] == ' ' || original[i + 1] == '\0') && original[i] != ' ')
			count++;
		i++;
	}
	return(count);
}

int	reverse(char **rev_str, int count, char *ori)
{
	int i = 0;
	int j;
	int k = count - 1;

	while(ori[i] != '\0')
	{
		while(ori[i] == ' ')
			i++;
		j = 0;
		while (!(ori[i+j] == ' ' || ori[i+j] == '\0'))
			j++;
		if (ori[i+j] == ' ' || ori[i+j] == '\0')
		{
			rev_str[k] = malloc(sizeof(char) * (j+1));
			if (!rev_str[k])
			{
				return(ft_free(rev_str));
			}
			strncpy(rev_str[k], ori + i, j);
			rev_str[k][j] = '\0';
			k--;
			i +=j;
		}
	}
	return (0);
}

int ft_free(char **rev_str)
{
	int i = 0;
	while(rev_str[i])
	{
		free(rev_str[i]);
		i++;
	}
	free(rev_str);
	return (-1);
}

void print_str(char **rev_str)
{
	int i = 0;
	int j;
	while(rev_str[i])
	{
		j = 0;
		while(rev_str[i][j])
		{
			write(1, &rev_str[i][j], 1);
			j++;
		}
		if (rev_str[i + 1])
			write(1, " ", 1);
		i++;
	}
}
