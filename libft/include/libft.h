/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:24:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/07 15:58:17 by anonymous     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h> // size_t
#include <stdbool.h> // bool

// ft_put.c
void ft_putchar(int c);
void ft_putchar_fd(int fd, int c);
void ft_putstr(const char *s);
void ft_putstr_fd(int fd, const char *s);
void ft_putendl(const char *s);
void ft_putendl_fd(int fd, const char *s);
// string.c
bool ft_strcmp(const char*s1, const char *s2);
bool ft_strncmp(const char*s1, const char *s2, size_t n);
size_t ft_strlen(const char *s);
char *ft_strsub(const char *str, size_t start, size_t size);
char *ft_strncpy(char *dst, const char *src, size_t len);
char *ft_strcpy(char *dst, const char *src);
char *ft_strdup(const char *src);
char *ft_strndup(const char *src, size_t size);