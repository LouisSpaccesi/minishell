/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:17:23 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:21:04 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countwords(const char *str, char c)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] != '\0')
		{
			count++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (count);
}

static void	freesplit(char **dest, int i)
{
	if (!dest)
		return ;
	while (i >= 0)
	{
		free(dest[i]);
		i--;
	}
	free(dest);
}

static int	ft_count_letter(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**split2(const char *s, char c, char **dest)
{
	int	i;
	int	len;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			len = ft_count_letter(s, c);
			dest[i] = malloc(sizeof(char) * (len + 1));
			if (!dest[i])
			{
				freesplit(dest, i - 1);
				return (NULL);
			}
			ft_strlcpy(dest[i], s, len + 1);
			s += len;
			i++;
		}
		else
			s++;
	}
	dest[i] = NULL;
	return (dest);
}

char	**ft_split(char const *s, char c)
{
	int		word_nb;
	char	**dest;

	if (!s)
		return (NULL);
	word_nb = countwords(s, c);
	dest = malloc(sizeof(char *) * (word_nb + 1));
	if (!dest)
		return (NULL);
	return (split2(s, c, dest));
}
