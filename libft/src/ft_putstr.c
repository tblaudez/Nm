/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putstr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:04:53 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:06:31 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write

size_t	ft_strlen(const char *s);

void	ft_putstr(const char *s) {
	write(STDOUT_FILENO, s, ft_strlen(s));
}