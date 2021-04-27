/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/14 11:44:40 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/27 10:51:33 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Create a new dynamically allocated list node and fill its content
t_list *ft_lstnew(void *content)
{
	t_list *lst;

	if (!(lst = ft_memalloc(sizeof(t_list))))
		return (NULL);

	lst->content = content;
	lst->next = NULL;

	return lst;
}

// Add node `new` as first node of the linked list pointed by `lst` 
void ft_lstadd_front(t_list **lst, t_list *new)
{
	t_list *tmp = *lst;

	*lst = new;
	new->next = tmp;
}

// Add node `new` to the end of the linked list pointed by `lst`
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

// Same as `add_front` but sets the `next` attribute of the node to NULL
// and returns said `next` attribute
t_list *ft_lstadd_front_null(t_list **lst, t_list *new)
{
	t_list *tmp = new->next;

	ft_lstadd_front(lst, new);
	new->next = NULL;

	return tmp;
}

// Same as `add_back` but sets the `next` attribute of the node to NULL
// and returns said `next` attribute
t_list *ft_lstadd_back_null(t_list **lst, t_list *new)
{
	t_list *tmp = new->next;

	ft_lstadd_back(lst, new);
	new->next = NULL;

	return tmp;
}

// Compute the size of a non-circular linked list
// `lst` is assumed to be the first node
int ft_lstsize(t_list *lst)
{
	int i = 0;

	while (lst) {
		i++;
		lst = lst->next;
	}

	return i;
}

// Return the last node of a non-circular linked list
t_list *ft_lstlast(t_list *lst)
{
	if (!lst)
		return NULL;

	while (lst->next)
		lst = lst->next;
	
	return lst;
}

// Deallocate a node and its content using free and `del`
void ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (del)
		del(lst->content);
	ft_memdel((void**)&lst);
}

// Deallocate all nodes of a linked list and their content
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

// Apply function `f` to the content attribute of each node of a linked list
void ft_lstiter(t_list *lst, void (*f)(void*))
{
	while (lst) {
		f(lst->content);
		lst = lst->next;
	}
}

// Create a new dynamically allocated linked list.
// Each content attribute of the new list will be the result of 
// calling `f` to the content attribute of each node of the "old" list
t_list *ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list *new_lst = NULL;
	t_list *node;

	while (lst) {
		if (!(node = ft_lstnew(f(lst->content)))) {
			ft_lstclear(&new_lst, del);
			return NULL;
		}
		ft_lstadd_back(&new_lst, node);
		lst = lst->next;
	}

	return new_lst;
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

// Merge sort a linked list using the `compare` function
void merge_sort_list(t_list **lst, int (*compare)(void *a, void *b))
{
	t_list *node = *lst;
	int lst_len;
	
	if ((lst_len = ft_lstsize(node)) <= 1)
		return;
	
	t_list *left = NULL;
	t_list *right = NULL;

	for (int i = 0; node; i++)
		node = ft_lstadd_back_null((i < lst_len / 2 ? &left : &right), node);
	
	merge_sort_list(&left, compare);
	merge_sort_list(&right, compare);

	*lst = merge_list(left, right, compare);
}