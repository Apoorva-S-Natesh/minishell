/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/19 14:20:18 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

int	main(int ac, char **av, char **envv)
{
    char    input[1000];
    int     i;
    i = 0;
    if (ac != 1 || av[1])
    {
        printf ("Program doesn't take arguments!");
        return (0);
    }
    while (envv[i] != NULL)
    {
        printf("ENV: %s\n", envv[i]);
        i++;
    }
	while (1)
	{
		//printDir();
		if (!takeInput(input))
			continue ;
		else
			printf("el input %s", input);
	}
	return (0);
}
