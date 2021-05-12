/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/11 11:42:43 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/11 11:42:44 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h> // bool
#include <stddef.h> // size_t
#include <stdlib.h> // free


size_t	ft_strlen(const char *s)
{	
	size_t len = 0;

	while(s[len])
		len++;

	return len;
}

int ft_strcmp(const char *s1, const char *s2)
{
	int	i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return (s1[i] - s2[i]);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i = 0;

	if (n == 0)
		return true;

	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;

	return (int)(s1[i] - s2[i]);
}

char *ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	for (i = 0; src[i]; i++)
		dst[i] = src[i];
	dst[i] = '\0';

	return dst;
}

char *ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	for (i = 0; src[i] && i < len; i++)
		dst[i] = src[i];
	for (; i < len; i++)
		dst[i] = '\0';

	return dst;
}

char *ft_strsub(const char *str, size_t start, size_t size)
{
	if (str == NULL || size == 0)
		return NULL;
	
	char *substr = (char*)ft_memalloc(size + 1);
	ft_strncpy(substr, str + start, size);

	return substr;
}

char *ft_strdup(const char *src)
{
	if (src == NULL)
		return NULL;
	char *str_dup = (char*)ft_memalloc(ft_strlen(src) + 1);
	ft_strcpy(str_dup, src);
	return str_dup;
}

char *ft_strndup(const char *src, size_t size)
{
	if (src == NULL)
		return NULL;
	char *str_dup = (char*)ft_memalloc(size);;
	return ft_strncpy(str_dup, src, size);
}

char *ft_strchr(const char *s, int c)
{
	if ((char)c == '\0')
		return ((char*)s + ft_strlen(s));
	for (int i = 0; s[i]; i++) {
		if (s[i] == (char)c)
			return (char*)s + i;
	}
	return NULL;
}

char *ft_strnchr(const char *s, int c, size_t size)
{
	for (size_t i = 0; s[i] && i < size; i++) {
		if (s[i] == (char)c)
			return (char*)s + i;
	}
	return NULL;
}

static t_list *create_word(const char *str, int c)
{
	t_word *word = (t_word*)ft_memalloc(sizeof(t_word));
	char *offset;

	word->ptr = str;
	if ((offset = ft_strchr(str, c)))
		word->size = (size_t)(offset - str);
	else
		word->size = ft_strlen(str);

	return ft_lstnew((void*)word);
}

char **ft_strsplit(const char *str, int c)
{
	t_list *word_list = NULL;

	if (!str)
		return NULL;

	while (str) {
		while (*str == (char)c)
			str++;
		if (!*str)
			break;
		ft_lstadd_back(&word_list, create_word(str, c));
		str = ft_strchr(str, c);
	}

	char **array = (char**)ft_memalloc(sizeof(char*) * (ft_lstsize(word_list) + 1));
	char **tmp = array;
	for (t_list *node = word_list; node ; node = node->next)
		*tmp++ = ft_strsub(((t_word*)node->content)->ptr, 0, ((t_word*)node->content)->size);
	
	ft_lstclear(&word_list, free);
	return array;
}