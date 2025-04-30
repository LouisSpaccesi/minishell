/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:50:34 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/20 23:18:45 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	s1_size;
	size_t	s2_size;

	if (s1 == NULL)
		return ((char *)s2);
	if (s2 == NULL)
		return ((char *)s1);
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	str = malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (!str)
		return (NULL);
	ft_memmove(str, s1, s1_size);
	ft_memmove(str + s1_size, s2, s2_size);
	str[s1_size + s2_size] = '\0';
	return (str);
}
