/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:24:51 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/10 12:11:53 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h> // size_t
#include <stdbool.h> // bool
#include <stdint.h> // uintmax_t


typedef struct {
	const char	*ptr;
	size_t 		size;
}				t_word;

typedef struct s_list {
	void			*content;
	struct s_list	*next;
}					t_list;


// character.c
bool ft_isspace(int c);
bool ft_isupper(int c);
bool ft_islower(int c);
bool ft_isalpha(int c);
bool ft_isdigit(int c);
bool ft_isalnum(int c);

// list.c
t_list *ft_lstnew(void *content);
void ft_lstadd_front(t_list **lst, t_list *new);
int ft_lstsize(t_list *lst);
t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **lst, t_list *new);
void ft_lstdelone(t_list *lst, void (*del)(void*));
void ft_lstclear(t_list **lst, void (*del)(void*));
void ft_lstiter(t_list *lst, void (*f)(void*));
t_list *ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*));
t_list *ft_lstadd_back_null(t_list **lst, t_list *new);
t_list *ft_lstadd_front_null(t_list **lst, t_list *new);
void merge_sort_list(t_list **lst, int (*compare)(void *a, void *b));

// memory.c
void *ft_memset(void *dest, int c, size_t len);
void *ft_memalloc(size_t size);
void ft_memdel(void **ap);
void *ft_memcpy(void *dst, const void *src, size_t n);

// number.c
int64_t ft_strtol(const char *str, char **endptr, int base);
int32_t ft_atoi(const char *str);
char *ft_itoa(int nbr);
char *ft_uitoa(unsigned int nbr);
char *ft_itoabase(uintmax_t nbr, int base, bool use_capitals);
int32_t ft_abs(int j);
int32_t ft_min(int a, int b);
int32_t ft_max(int a, int b);

// ft_put.c
void ft_putchar(int c);
void ft_putchar_fd(int fd, int c);
void ft_putstr(const char *s);
void ft_putstr_fd(int fd, const char *s);
void ft_putendl(const char *s);
void ft_putendl_fd(int fd, const char *s);

// string.c
int ft_strcmp(const char*s1, const char *s2);
int ft_strncmp(const char*s1, const char *s2, size_t n);
size_t ft_strlen(const char *s);
char *ft_strsub(const char *str, size_t start, size_t size);
char *ft_strncpy(char *dst, const char *src, size_t len);
char *ft_strcpy(char *dst, const char *src);
char *ft_strdup(const char *src);
char *ft_strndup(const char *src, size_t size);
char *ft_strchr(const char *s, int c);
char *ft_strnchr(const char *s, int c, size_t size);
char **ft_strsplit(const char *str, int c);