/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   apply_operator.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:30:02 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/13 14:30:45 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h> // uintmax_t
#include "ft_fprintf.h"
#include "libft.h"

void apply_dec_operators(char **aptr, uintmax_t value, unsigned int flags)
{
	int int_value = (int)value;

	if (int_value >= 0) {
		if (flags & PLUS)
			*((*aptr)++) = '+';
		else if (flags & BLANK)
			*((*aptr)++) = ' ';
	} else
		*((*aptr)++) = '-';
}

void apply_hex_operator(char **aptr, uintmax_t value, unsigned int flags)
{
	(void)value;

	if (flags & HASHTAG) {
		ft_strcpy(*aptr, ((flags & CAPITAL) ? "0X" : "0x"));
		*aptr += 2;
	}
}

void apply_octal_operator(char **aptr, uintmax_t value, unsigned int flags)
{
	(void)value;

	if (flags & HASHTAG) {
		*((*aptr)++) = '0';
	}
}

void apply_binary_operator(char **aptr, uintmax_t value, unsigned int flags)
{
	(void)value;

	if (flags & HASHTAG) {
		ft_strcpy(*aptr, "0b");
		*aptr += 2;
	}
}