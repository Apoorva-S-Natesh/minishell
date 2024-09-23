/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/23 14:19:55 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
   free_all(&mini);
	return (0);
}
