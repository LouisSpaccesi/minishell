/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:18:49 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:18:50 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_negative(int nb)
{
	if (nb < 0)
		return (1);
	return (0);
}

static int	nb_len(int nb)
{
	int	i;

	i = 0;
	if (nb == 0)
		return (1);
	while (nb)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

static void	convertstr(int n, char *tab, int i)
{
	char	c;

	while (i--)
	{
		c = (n % 10 + '0');
		tab[i] = c;
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*tab;
	int		i;
	int		negative;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	negative = is_negative(n);
	tab = malloc(nb_len(n) + negative + 1);
	if (!tab)
		return (NULL);
	if (negative)
		n = -n;
	i = nb_len(n) + negative;
	tab[i] = '\0';
	convertstr(n, tab, i);
	if (negative)
		tab[0] = '-';
	return (tab);
}
