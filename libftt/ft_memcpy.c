/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:45:44 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:15:29 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*dst;
	const char	*sc;

	dst = (char *)dest;
	sc = (const char *)src;
	i = 0;
	if (dst == NULL && sc == NULL)
		return (NULL);
	while (n--)
	{
		dst[i] = sc[i];
		i++;
	}
	return ((void *)dst);
}
