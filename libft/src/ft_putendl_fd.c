/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putendl_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:07:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:09:28 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

void	ft_putstr_fd(const char *s, int fd);
void	ft_putchar_fd(int c, int fd);

void	ft_putendl_fd(const char *s, int fd) {
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}