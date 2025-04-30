/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:20:04 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:20:05 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *)s;
	i = ft_strlen(s);
	if (!(unsigned char)c)
		return ((char *)(s + i));
	while (i--)
	{
		if (str[i] == (unsigned char)c)
		{
			return ((char *)&str[i]);
		}
	}
	if ((unsigned char)c == '\0')
		return ((char *)&str[i]);
	return (NULL);
}
