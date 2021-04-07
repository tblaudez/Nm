/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 14:15:20 by anonymous     #+#    #+#                 */
/*   Updated: 2021/04/07 15:57:57 by anonymous     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h> // bool
#include <stddef.h> // size_t
#include <stdlib.h> // calloc

#define MIN(a, b) (a < b ? a : b)

size_t	ft_strlen(const char *s) {
	
	size_t len = 0;
	while (*s++)
		len++;
	return len;
}

bool ft_strcmp(const char *s1, const char *s2)
{
	size_t	i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return !(bool)(s1[i] - s2[i]);
}

bool ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i = 0;

	if (n == 0)
		return true;

	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;

	return !(bool)(s1[i] - s2[i]);
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
	for (; i <= len; i++)
		dst[i] = '\0';

	return dst;
}

char *ft_strsub(const char *str, size_t start, size_t size)
{
	if (str == NULL || size == 0)
		return NULL;
	
	char *substring = (char*)calloc(1, size);
	ft_strncpy(substring, str + start, size);

	return substring;
}

char *ft_strdup(const char *src)
{
	if (src == NULL)
		return NULL;
	char *str_dup = (char*)calloc(1, ft_strlen(src));
	ft_strcpy(str_dup, src);
	return str_dup;
}

char *ft_strndup(const char *src, size_t size)
{
	if (src == NULL)
		return NULL;
	char *str_dup = (char*)calloc(1, MIN(size, ft_strlen(src)));
	ft_strncpy(str_dup, src, MIN(size, ft_strlen(src)));
	return str_dup;
}