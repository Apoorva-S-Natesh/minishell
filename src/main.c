/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/23 11:39:52 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}*/

void    init_shell(t_shell *mini, char **envv)
{
    mini->running_status = 1;
    printf("the status is %d\n", mini->running_status);
    set_envv(envv);
}

void    set_envv(char **envv)
{
    int i;

    i = 0;
    while (envv[i] != NULL)
    {
        printf("ENV: %s\n", envv[i]);
        i++;
    }

}
int	main(int ac, char **av, char **envv)
{
    t_shell mini;


    if (!check_args(ac, av))
        return (0);
    init_shell(&mini, envv);
    //printDir();
    /*loop
    while (mini.status)
    {
        intialize_shell();
        // read input
        //tokenize
        //execute
    }
    */

	return (0);
}
