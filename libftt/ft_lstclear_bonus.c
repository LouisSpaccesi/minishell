/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:42:52 by lospacce          #+#    #+#             */
/*   Updated: 2024/10/19 19:15:02 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*occ;

	if (!lst || !del)
		return ;
	if (!*lst)
		return ;
	occ = *lst;
	while (occ)
	{
		temp = occ->next;
		ft_lstdelone(occ, del);
		occ = temp;
	}
	*lst = NULL;
}
