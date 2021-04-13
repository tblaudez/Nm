/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   character.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 09:36:38 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/08 10:11:38 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h> // bool

bool ft_isspace(int c)
{
	return (unsigned char)c == '\t' || (unsigned char)c == '\n' \
		|| (unsigned char)c == '\v' || (unsigned char)c == '\f' \
		|| (unsigned char)c == '\r' || (unsigned char)c == ' ';
}

bool ft_isupper(int c)
{
	return ((unsigned char)c >= 'A' && (unsigned char)c <= 'Z');
}

bool ft_islower(int c)
{
	return ((unsigned char)c >= 'a' && (unsigned char)c <= 'z');
}

bool ft_isalpha(int c)
{
	return ft_isupper(c) || ft_islower(c);
}

bool ft_isdigit(int c)
{
	return (unsigned char)c >= '0' && (unsigned char)c <= '9';
}

bool ft_isalnum(int c)
{
	return ft_isalpha(c) || ft_isdigit(c);
}

