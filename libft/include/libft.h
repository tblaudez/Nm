/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:24:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/03/31 14:10:05 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h> // size_t

size_t	ft_strlen(const char *s);
void	ft_putchar(int c);
void	ft_putchar_fd(int c, int fd);
void	ft_putstr(const char *s);
void	ft_putstr_fd(const char *s, int fd);
void	ft_putendl(const char *s);
void	ft_putendl_fd(const char *s, int fd);