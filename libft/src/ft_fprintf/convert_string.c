/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert_string.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:57:14 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/13 15:18:41 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h> // va_arg
#include <stddef.h> // NULL
#include "ft_fprintf.h"
#include "libft.h"

extern va_list g_ap;

static unsigned char get_flags(const char *str, char **aptr)
{
	unsigned char flags = 0;
	int i;

	for (i = 1; ft_strchr(FLAGS, str[i]); i++) {
		switch (str[i]) {
			case '-':
				flags |= MINUS;
				flags &= ~ZERO;
				break;
			case '0':
				if (!(flags & MINUS))
					flags |= ZERO;
				break;
			case '+':
				flags |= PLUS;
				flags &= ~BLANK;
				break;
			case ' ':
				if (!(flags & PLUS))
					flags |= BLANK;
				break;
			case '#':
				flags |= HASHTAG;
				break;
			default:
				break;
		}
	}

	*aptr = (char*)str + i;
	return flags;
}

static unsigned int get_width(const char *str, char **aptr, unsigned char *flags)
{
	if (*str == '*') {
		int width_arg = va_arg(g_ap, int);
		if (width_arg < 0)
			*flags |= MINUS;
		return ft_abs(width_arg);
	}

	return (unsigned int)ft_strtol(str, aptr, 10);
}

char *convert_string(const char *str)
{
	char *width_ptr = NULL, *format_ptr = NULL;
	unsigned char flags = get_flags(str, &width_ptr);
	unsigned int width = get_width(width_ptr, &format_ptr, &flags);

	switch (*format_ptr) {
		case 'd':
		case 'i':
			return get_int_value(flags, width);
		case 'X':
			flags |= CAPITAL;
		case 'p':
			flags |= HASHTAG;
		case 'x':
			return get_hex_value(flags, width);
		case 'o':
			return get_octal_value(flags, width);
		case 'b':
			return get_binary_value(flags, width);
		case 'c':
			return get_char_value(flags, width);
		case 's':
			return get_string_value(flags, width);
		case '%':
			return ft_strdup("%");
		default:
			return NULL;
	}
}