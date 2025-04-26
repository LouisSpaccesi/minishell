/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:04:38 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/26 17:48:43 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_env_value(char *var_name, char **envp)
{
    int i;
    size_t len;
    
    len = ft_strlen(var_name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

static void print_env_var(char *var_name, char **envp)
{
    char *value;
    
    if (ft_strncmp(var_name, "PWD", 3) == 0)
        ft_pwd_no_nl();
    else
    {
        value = get_env_value(var_name, envp);
        if (value)
            printf("%s", value);
    }
}

static void print_echo_arg(char *arg, char **envp)
{
    int     len;
    char    temp[1024];
    
    len = ft_strlen(arg);
    if (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'')
    {
        ft_strcpy(temp, arg + 1);
        temp[len - 2] = '\0';
        printf("%s", temp);
    }
    else if (len >= 2 && arg[0] == '"' && arg[len - 1] == '"')
    {
        ft_strcpy(temp, arg + 1);
        temp[len - 2] = '\0';
        if (temp[0] == '$')
            print_env_var(temp + 1, envp);
        else
            printf("%s", temp);
    }
    else if (arg[0] == '$')
        print_env_var(arg + 1, envp);
    else
        printf("%s", arg);
}


static int check_no_newline(int argc, char **argv, int *i)
{
    int no_newline;
    
    no_newline = 0;
    if (*i < argc && ft_strncmp(argv[*i], "-n", 3) == 0)
    {
        no_newline = 1;
        (*i)++;
    }
    return (no_newline);
}

static void print_echo_args(int argc, char **argv, int i, char **envp)
{
    while (i < argc)
    {
        print_echo_arg(argv[i], envp);
        if (i + 1 < argc)
            printf(" ");
        i++;
    }
}

int ft_echo(int argc, char **argv, char **envp)
{
    int i;
    int no_newline;
    
    i = 1;
    no_newline = check_no_newline(argc, argv, &i);
    print_echo_args(argc, argv, i, envp);
    if (!no_newline)
        printf("\n");
    return (0);
}