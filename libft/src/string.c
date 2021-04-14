/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 14:15:20 by anonymous     #+#    #+#                 */
/*   Updated: 2021/04/14 13:21:37 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h> // bool
#include <stddef.h> // size_t

void *ft_memalloc(size_t size);


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