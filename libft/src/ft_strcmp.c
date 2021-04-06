/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 13:39:49 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/06 14:10:28 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h> // bool
#include <stddef.h> // size_t

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