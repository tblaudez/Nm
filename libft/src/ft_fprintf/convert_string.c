/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert_string.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:57:14 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/06 12:00:59 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <stddef.h> // NULL

static void get_flags(const char **aptr, size_t size, uint8_t *aflags)
{
	int i;

	for (i = 1; ft_strnchr(FLAGS, (*aptr)[i], size); i++) {
		switch ((*aptr)[i]) {
			case '-':
				*aflags |= MINUS;
				*aflags &= ~ZERO;
				break;
			
			case '0':
				if (!(*aflags & MINUS))
					*aflags |= ZERO;
				break;
			
			case '+':
				*aflags |= PLUS;
				*aflags &= ~BLANK;
				break;
		
			case ' ':
				if (!(*aflags & PLUS))
					*aflags |= BLANK;
				break;
			
			case '#':
				*aflags |= HASHTAG;
				break;
			
			default:
				break;
		}
	}

	*aptr += i;
}

static void get_width(const char **aptr, size_t size, uint32_t *awidth, uint8_t *aflags)
{
	char buf[size + 1];
	char *endptr;

	if (**aptr == '*') {
		if ((*awidth = va_arg(g_ap, int)) < 0) {
			*aflags |= MINUS;
			*awidth = ft_abs(*awidth);
		}
		*aptr += 1;
		return;
	}

	ft_strncpy(buf, *aptr, size + 1);
	*awidth = (uint32_t)ft_strtol(buf, &endptr, 10);
	*aptr += (uintptr_t)(endptr - buf);
}

void format_and_print_string(const char *str, size_t size)
{
	unsigned char flags;
	unsigned int width;

	get_flags(&str, size, &flags);
	get_width(&str, size, &width, &flags);

	switch (*str) {
		case 'd':
		case 'i':
			print_int_value(flags, width); break;
		case 'X':
			flags |= CAPITAL;
		case 'p':
			flags |= HASHTAG;
		case 'x':
			print_hex_value(flags, width); break;
		case 'o':
			print_octal_value(flags, width); break;
		case 'b':
			print_binary_value(flags, width); break;
		case 'c':
			print_char_value(flags, width); break;
		case 's':
			print_string_value(flags, width); break;
		case '%':
			ft_putstr_fd(g_fd, "%"); break;
		default:
			break;
	}
}