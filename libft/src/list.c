/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/14 11:44:40 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/14 14:51:08 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list *ft_lstnew(void *content)
{
	t_list *lst;

	if (!(lst = ft_memalloc(sizeof(t_list))))
		return (NULL);

	lst->content = content;
	lst->next = NULL;

	return lst;
}

void ft_lstadd_front(t_list **lst, t_list *new)
{
	t_list *tmp = *lst;

	*lst = new;
	new->next = tmp;
}

int ft_lstsize(t_list *lst)
{
	int i = 0;

	do {
		i++;
		lst = lst->next;
	} while (lst);

	return i;
}

t_list *ft_lstlast(t_list *lst)
{
	while (lst->next)
		lst = lst->next;
	
	return lst;
}

void ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *node = *lst;

	if (!node) {
		*lst = new;
		return;
	}

	node = ft_lstlast(node);
	node->next = new;
}

void ft_lstdelone(t_list *lst, void (*del)(void*))
{
	del(lst->content);
	ft_memdel((void**)&lst);
}

void ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list *node = *lst;
	t_list *tmp;

	while (node) {
		tmp = node;
		node = node->next;
		ft_lstdelone(tmp, del);
	}

	*lst = NULL;
}

void ft_lstiter(t_list *lst, void (*f)(void*))
{
	do {
		f(lst->content);
		lst = lst->next;
	} while (lst);
}

t_list *ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list *new_lst = NULL;
	t_list *node;

	do {
		if (!(node = ft_lstnew(f(lst->content)))) {
			ft_lstclear(&new_lst, del);
			return NULL;
		}
		ft_lstadd_back(&new_lst, node);
		lst = lst->next;
	} while (lst);

	return new_lst;
}