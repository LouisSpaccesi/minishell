/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:20:09 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:20:10 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	before(char const *str, char const *set)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (set[j])
		{
			if (str[i] == set[j])
				break ;
			j++;
		}
		if (!set[j])
			break ;
		i++;
	}
	return (i);
}

static int	after(char const *str, char const *set)
{
	int	i;
	int	j;

	i = ft_strlen(str);
	i--;
	while (i >= 0)
	{
		j = 0;
		while (set[j])
		{
			if (str[i] == set[j])
				break ;
			j++;
		}
		if (!set[j])
			break ;
		i--;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*str;
	int		size;

	if (!s1 | !set)
		return (NULL);
	i = before(s1, set);
	j = after(s1, set);
	if (i > j)
		return (ft_strdup(""));
	size = j - i + 1;
	str = malloc(size + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[i], size + 1);
	return (str);
}
