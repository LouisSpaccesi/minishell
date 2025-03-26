/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:22:23 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/25 15:09:38 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <stdio.h>

/*
Function takes error code as argument to be used at anytime time.
EXIT_FAILURE = 1, EXIT_SUCCESS = 0
*/
void ft_exit(int EXIT_CODE)
{
	printf("%d", EXIT_CODE);
	exit(EXIT_CODE);
}

// int main()
// {
// 	ft_exit(EXIT_FAILURE);
// }