/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:17:26 by fben-ham          #+#    #+#             */
/*   Updated: 2025/03/26 16:48:03 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_received_signal = 0;

void signal_handle(int sig)
{
    
}

void setup_signal()
{
    struct sigaction action1; // CTRL+C
    struct sigaction action2; // CTRL+/
    
    sigemptyset(&action1.sa_mask);
    action1.sa_flags = 0;
    action1.sa_handler = signal_handle;
    action1.sa_sigaction(SIGINT, &action1 ,NULL);

    sigemptyset(&action2.sa_mask);
    action1.sa_flags = 0;
    action1.sa_handler = signal_handle;
    action1.sa_sigaction(SIGQUIT, &action2,NULL);
    

}
/*
Main respectant uniquement la phase 1 pour le moment

Note que Break -> coupe en cas EOF
        ajouter a l'historique uniquement si la chaine n'est pas vide
TODO : 
*/
int main()
{
    char *rl;

    setup_signal(); // regroupe sigaction pour les differents cas : ctrl+c et ctrl+/
    while(1)
    {
        rl = readline("MissionLocale> ");
        if(!rl)
            break; // en cas de EOF (ctrl+d)
        if(*rl) // uniquement si pleine
            add_history(rl);
        free(rl);
    }   
    return (0);
}