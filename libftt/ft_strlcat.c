/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:19:28 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:19:29 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t		i;
	size_t		j;
	char		*d;
	const char	*s;

	if ((dst == 0 || src == 0) && size == 0)
		return (0);
	d = dst;
	s = (void *)src;
	i = ft_strlen(dst);
	j = 0;
	if (size <= i)
		return (size + ft_strlen((s)));
	while (s[j] && j + i < size - 1)
	{
		d[i + j] = s[j];
		j++;
	}
	d[i + j] = '\0';
	return (i + ft_strlen(s));
}
