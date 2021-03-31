/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putchar_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:00:54 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:13:20 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write

void	ft_putchar_fd(int c, int fd) {
	unsigned char c_dup = (unsigned char)c;
	write(fd, &c_dup, 1);
}