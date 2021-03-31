/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlen.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:25:58 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:14:23 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h> // size_t

size_t	ft_strlen(const char *s) {
	
	size_t len = 0;
	while (*s++)
		len++;
	return len;
}