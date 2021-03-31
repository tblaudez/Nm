/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putstr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:04:53 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:07:26 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write

size_t	ft_strlen(const char *s);

void	ft_putstr_fd(const char *s, int fd) {
	write(fd, s, ft_strlen(s));
}