/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:42:23 by anonymous     #+#    #+#                 */
/*   Updated: 2021/05/06 10:47:12 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <stdlib.h> // free

va_list g_ap;
int g_fd;

void ft_fprintf(int fd, const char *format, ...)
{
	g_fd = fd;
	t_list *format_list = NULL;

	va_start(g_ap, format);

	format_list = create_format_list(format);
	ft_lstiter(format_list, print_format);	
	ft_lstclear(&format_list, free);

	va_end(g_ap);
}