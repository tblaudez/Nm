/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 13:27:37 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/08 13:40:24 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h> // malloc, free
#include <stddef.h> // size_t

void *ft_memset(void *dest, int c, size_t len)
{
	unsigned char *ptr = (unsigned char*)dest;
	while (len-- > 0)
		*ptr++ = (unsigned char)c;
	return dest;
}

void *ft_memalloc(size_t size)
{
	void *ptr;

	if (!(ptr = malloc(size)))
		return NULL;
	return ft_memset(ptr, 0, size);
}

void ft_memdel(void **ap)
{
	free(*ap);
	*ap = NULL;
}