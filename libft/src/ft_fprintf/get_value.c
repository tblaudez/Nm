/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_value.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:31:02 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/13 14:32:37 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h> // uintmax_t
#include <stdarg.h> // va_arg
#include "ft_fprintf.h"
#include "libft.h"

extern va_list g_ap;


static char *get_filled_string(uintmax_t value, const char *str_value, 
	const int str_value_len, const int operator_len, unsigned char flags,
	unsigned int width, void (*apply_operator)(char **, uintmax_t, unsigned int))
{

	width = ft_max(width - str_value_len - operator_len, 0);
	char *buf = (char*)ft_memalloc(str_value_len + operator_len + width + 1);
	char *ptr = buf;

	if (flags & MINUS) {
		if (apply_operator)
			apply_operator(&ptr, value, flags);
		ft_strcpy(ptr, str_value);
		ft_memset(ptr + str_value_len, ' ', width);
	} else if (flags & ZERO) {
		if (apply_operator)
			apply_operator(&ptr, value, flags);
		ft_memset(ptr, '0', width);
		ft_strcpy(ptr + width, str_value);
	} else {
		ft_memset(ptr, ' ', width);
		ptr += width;
		if (apply_operator)
			apply_operator(&ptr, value, flags);
		ft_strcpy(ptr, str_value);
	}
	
	return buf;
}

char *get_int_value(unsigned char flags, unsigned int width)
{
	int value = va_arg(g_ap, int);
	const char *str_value = ft_itoa(ft_abs(value));
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & (PLUS|BLANK)) || value < 0) ? 1 : 0;

	return get_filled_string(value, str_value, str_value_len, operator_len, flags, width, &apply_dec_operators);
}

char *get_hex_value(unsigned char flags, unsigned int width)
{
	unsigned long value = va_arg(g_ap, unsigned long);
	const char *str_value = ft_itoabase(value, 16, flags & CAPITAL);
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & HASHTAG) && value != 0 ? 2 : 0);

	return get_filled_string(value, str_value, str_value_len, operator_len, flags, width, &apply_hex_operator);
}

char *get_octal_value(unsigned char flags, unsigned int width)
{
	unsigned long value = va_arg(g_ap, unsigned long);
	const char *str_value = ft_itoabase(value, 8, false);
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & HASHTAG) && value != 0 ? 1 : 0);

	return get_filled_string(value, str_value, str_value_len, operator_len, flags, width, &apply_octal_operator);
}

char *get_binary_value(unsigned char flags, unsigned int width)
{
	unsigned long value = va_arg(g_ap, unsigned long);
	const char *str_value = ft_itoabase(value, 2, false);
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & HASHTAG) && value != 0 ? 2 : 0);

	return get_filled_string(value, str_value, str_value_len, operator_len, flags, width, &apply_binary_operator);
}

char *get_char_value(unsigned char flags, unsigned int width)
{
	int value = va_arg(g_ap, int);
	const char str_value[] = {value, 0};
	const int str_value_len = 1;
	const int operator_len = 0;

	flags &= MINUS; // All flags are ignored but '-'

	return get_filled_string(value, str_value, str_value_len, operator_len, flags, width, NULL);
}

char *get_string_value(unsigned char flags, unsigned int width)
{
	char* value = va_arg(g_ap, char*);
	const char *str_value = value;
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = 0;

	flags &= MINUS; // All flags are ignored but '-'

	return get_filled_string(0, str_value, str_value_len, operator_len, flags, width, NULL);
}