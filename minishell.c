/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:17:26 by fben-ham          #+#    #+#             */
/*   Updated: 2025/03/26 15:57:02 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/*
Main respectant uniquement la phase 1 pour le moment

Note que Break -> coupe en cas EOF
        ajouter a l'historique uniquement si la chaine n'est pas vide
TODO : 
*/
int main()
{
    char *rl;

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