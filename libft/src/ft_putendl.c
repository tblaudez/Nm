/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putendl.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:07:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:08:31 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

void	ft_putstr(const char *s);
void	ft_putchar(int c);

void	ft_putendl(const char *s) {
	ft_putstr(s);
	ft_putchar('\n');
}