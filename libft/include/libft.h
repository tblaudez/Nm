/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:24:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/06 13:51:36 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h> // size_t
#include <stdbool.h> // bool


// ft_put.c
void ft_putchar(int c);
void ft_putchar_fd(int c, int fd);
void ft_putstr(const char *s);
void ft_putstr_fd(const char *s, int fd);
void ft_putendl(const char *s);
void ft_putendl_fd(const char *s, int fd);
// ft_strcmp.c
bool ft_strcmp(const char*s1, const char *s2);
bool ft_strncmp(const char*s1, const char *s2, size_t n);
// ft_strlen.c
size_t ft_strlen(const char *s);