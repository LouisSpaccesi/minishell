#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

extern int g_received_signal; // extend permet d'etendre la variable dans plusieurs fichiers sans redefinir constamment

#endif