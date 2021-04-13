/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:24:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/13 09:56:24 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include <stdint.h> // uintmax_t

// character.c
bool ft_isspace(int c);
bool ft_isupper(int c);
bool ft_islower(int c);
bool ft_isalpha(int c);
bool ft_isdigit(int c);
bool ft_isalnum(int c);
// memory.c
void *ft_memset(void *dest, int c, size_t len);
void *ft_memalloc(size_t size);
void ft_memdel(void **ap);
// number.c
long ft_strtol(const char *str, char **endptr, int base);
int ft_atoi(const char *str);
char *ft_itoa(int nbr);
char *ft_uitoa(unsigned int nbr);
char *ft_itoabase(uintmax_t nbr, int base, bool use_capitals);
int ft_abs(int j);
int ft_min(int a, int b);
int ft_max(int a, int b);
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
char *ft_strchr(const char *s, int c);