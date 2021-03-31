/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putchar.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:00:54 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:13:45 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // write

void	ft_putchar(int c) {
	unsigned char c_dup = (unsigned char)c;
	write(STDOUT_FILENO, &c_dup, 1);
}