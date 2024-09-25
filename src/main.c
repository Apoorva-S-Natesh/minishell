/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:41:01 by aschmidt          #+#    #+#             */
/*   Updated: 2024/09/25 09:54:34 by aschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void print_tokens(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        printf("Token Type: %d, Token Value: %s\n", current->type, current->value);
        current = current->next;
    }
}

int	main(int ac, char **av, char **envv)
{
    t_shell mini;
    t_token *tokens;

    if (!check_args(ac, av))
        return (0);
    init_shell(&mini, envv);
    while (mini.running_status)
    {
        if (take_input(&mini))
        {
            printf("hay input!: %s\n", mini.input);
            tokens = tokenize(mini.input);
            print_tokens(tokens);
            //execute
        }
    }
   free_all(&mini);
	return (0);
}
