/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:17:26 by fben-ham          #+#    #+#             */
/*   Updated: 2025/03/26 17:31:42 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_received_signal = 0;

/*
Ctrl+c : nouvelle ligne, readline est informé, vide toute la ligne, reaffiche le prompt
Ctrl+/ : rien ne se passe comme demandé dans le sujet
*/
void signal_handle(int sig)
{
    if(sig == SIGINT)
    {
        g_received_signal = SIGINT;

        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

/*
Setup le traitement des signaux pour ctrl c et / 
*/
void setup_signal(void)
{
    struct sigaction action;
    
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = signal_handle;
    sigaction(SIGINT, &action,NULL); // CTRL+C

    signal(SIGQUIT, SIG_IGN);
}

/*
Gestion au cas où une commande est en cours
Mise à default
*/
void set_noninteractive_signal(void)
{
    struct sigaction action;

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = SIG_DFL;
    sigaction(SIGINT, &action,NULL);
    sigaction(SIGQUIT, &action,NULL);
}


/*
Main respectant uniquement la phase 1 pour le moment

Note que Break -> coupe en cas EOF
        ajouter a l'historique uniquement si la chaine n'est pas vide
TODO / Notes :
- Si probleme avec l'interpret de RL pour les signaux -> rl_catch_signals = 0
-  
*/
int main()
{
    char *rl;

    
    while(1)
    {
        setup_signal(); // regroupe sigaction pour les differents cas : ctrl+c et ctrl+/
        rl = readline("MissionLocale> ");
        if(!rl)
            break; // en cas de EOF (ctrl+d)
        if(*rl) // uniquement si pleine
        {
            set_noninteractive_signal();
            add_history(rl);
        }

        free(rl);
    }  
    return (0);
}