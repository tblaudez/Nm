/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   put.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:00:54 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/10 11:35:08 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h> // write

void ft_putchar(int c)
{
	write(STDOUT_FILENO, (unsigned char*)&c, 1);
}

void ft_putchar_fd(int fd, int c)
{
	write(fd, (unsigned char*)&c, 1);
}

void ft_putstr(const char *s)
{
	write(STDOUT_FILENO, s, ft_strlen(s));
}

void ft_putstr_fd(int fd, const char *s)
{
	write(fd, s, ft_strlen(s));
}

void ft_putendl(const char *s)
{
	ft_putstr(s);
	ft_putchar('\n');
}

void ft_putendl_fd(int fd, const char *s) {
	ft_putstr_fd(fd, s);
	ft_putchar_fd(fd, '\n');
}