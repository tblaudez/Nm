/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_put.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:00:54 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/06 13:49:52 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write

size_t	ft_strlen(const char *s);


void ft_putchar(int c)
{
	write(STDOUT_FILENO, (unsigned char*)&c, 1);
}

void ft_putchar_fd(int c, int fd)
{
	write(fd, (unsigned char*)&c, 1);
}

void ft_putstr(const char *s)
{
	write(STDOUT_FILENO, s, ft_strlen(s));
}

void ft_putstr_fd(const char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

void ft_putendl(const char *s)
{
	ft_putstr(s);
	ft_putchar('\n');
}

void ft_putendl_fd(const char *s, int fd) {
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}