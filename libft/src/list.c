/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/14 11:44:40 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/15 10:16:46 by tblaudez      ########   odam.nl         */
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

t_list *ft_lstadd_back_null(t_list **lst, t_list *new)
{
	t_list *tmp = new->next;

	ft_lstadd_back(lst, new);
	new->next = NULL;

	return tmp;
}

t_list *ft_lstadd_front_null(t_list **lst, t_list *new)
{
	t_list *tmp = new->next;

	ft_lstadd_front(lst, new);
	new->next = NULL;

	return tmp;
}

static t_list *merge_list(t_list *left, t_list *right, int (*compare)(void *a, void *b))
{
	t_list *result = NULL;

	while (left && right) {
		if (compare(left->content, right->content) <= 0)
			left = ft_lstadd_back_null(&result, left);
		else
			right = ft_lstadd_back_null(&result, right);
	}

	while (left)
		left = ft_lstadd_back_null(&result, left);
	while (right)
		right = ft_lstadd_back_null(&result, right);

	return result;
}

void merge_sort_list(t_list **lst, int (*compare)(void *a, void *b))
{
	t_list *node = *lst;
	int lst_len;
	
	if ((lst_len = ft_lstsize(node)) <= 1)
		return;
	
	t_list *left = NULL;
	t_list *right = NULL;

	for(int i = 0; node; i++)
		node = ft_lstadd_back_null((i < lst_len / 2 ? &left : &right), node);
	
	merge_sort_list(&left, compare);
	merge_sort_list(&right, compare);

	*lst = merge_list(left, right, compare);
}