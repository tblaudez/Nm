/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   number.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 09:23:37 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/10 11:34:58 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h> // bool
#include <stddef.h> // size_t
#include <limits.h> // LONG_MAX LONG_MIN
#include <stdint.h> // uintmax_t

#define LONG_DIGITS 19
#define ULONG_DIGITS 20

int64_t ft_strtol(const char *str, char **endptr, int base)
{
	const char *s = str;
	int c, neg = 0, any, cutlim;
	unsigned long cutoff, acc;

	do {
		c = *s++;
	} while (ft_isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X')) {
			c = s[1];
			s += 2;
			base = 16;		
	}
	if (base == 0)
		base = (c == '0' ? 8 : 10);

	cutoff = (neg ? -(unsigned long)LONG_MIN : LONG_MAX);
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (ft_isdigit(c))
			c -= '0';
		else if (ft_isalpha(c))
			c -= (ft_isupper(c) ? 'A' - 10 : 'a' - 10);
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0)
		acc = (neg ? LONG_MIN : LONG_MAX);
	else if (neg)
		acc = -acc;
	if (endptr)
		*endptr = (char*)(any ? s - 1 : str);
	return acc;
}

int32_t ft_atoi(const char *str)
{
	return (int)ft_strtol(str, (char**)NULL, 10);
}

char *ft_itoa(int nbr)
{
	static char buf[LONG_DIGITS + 2];
	char *p = buf + LONG_DIGITS + 1;
	bool neg = (nbr < 0);

	do {
		int modulo = (nbr % 10);
		*--p = '0' + (nbr >= 0 ? modulo : -modulo);
		nbr /= 10;
	} while (nbr != 0);
	if (neg)
		*--p = '-';
	return p;
	
}

char *ft_uitoa(unsigned int nbr)
{
	static char buf[ULONG_DIGITS + 1];
	char *p = buf + ULONG_DIGITS;

	do {
		*--p = '0' + (nbr % 10);
		nbr /= 10;
	} while (nbr != 0);
	return p;
}

char *ft_itoabase(uintmax_t nbr, int base, bool use_capitals)
{
	static char buf[ULONG_DIGITS + 2];
	char *p = buf + ULONG_DIGITS + 1;
	static char digits[] = "0123456789abcdef0123456789ABCDEF";
	
	if (base <= 10)
		use_capitals = false;

	do {
		*--p = digits[(nbr % base) + (use_capitals ? 16 : 0)];
		nbr /= base;
	} while (nbr != 0);
	return p;
}

int32_t ft_abs(int j)
{
	return (j >= 0 ? j : -j);
}

int32_t ft_min(int a, int b)
{
	return (a < b ? a : b);
}

int32_t ft_max(int a, int b)
{
	return (a > b ? a : b);
}