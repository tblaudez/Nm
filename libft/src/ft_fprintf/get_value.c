/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_value.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:31:02 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/06 10:40:04 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <stdint.h> // uintmax_t

static void print_formated_string(uintmax_t value, const char *str_value, \
	const int str_value_len, const int operator_len, unsigned char flags, \
	unsigned int width, void (*apply_operator)(char **, uintmax_t, unsigned int))
{
	width = ft_max(width - str_value_len - operator_len, 0);
	size_t bufsize = str_value_len + operator_len + width + 2;
	char buf[bufsize];
	char *ptr = buf;

	ft_memset(buf, 0, bufsize);

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
	
	ft_putstr_fd(g_fd, buf);
}

void print_int_value(unsigned char flags, unsigned int width)
{
	int value = va_arg(g_ap, int);
	const char *str_value = ft_itoa(ft_abs(value));
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & (PLUS|BLANK)) || value < 0) ? 1 : 0;

	print_formated_string(value, str_value, str_value_len, operator_len, flags, width, &apply_dec_operators);
}

void print_hex_value(unsigned char flags, unsigned int width)
{
	unsigned long value = va_arg(g_ap, unsigned long);
	const char *str_value = ft_itoabase(value, 16, flags & CAPITAL);
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & HASHTAG) && value != 0 ? 2 : 0);

	print_formated_string(value, str_value, str_value_len, operator_len, flags, width, &apply_hex_operator);
}

void print_octal_value(unsigned char flags, unsigned int width)
{
	unsigned long value = va_arg(g_ap, unsigned long);
	const char *str_value = ft_itoabase(value, 8, false);
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & HASHTAG) && value != 0 ? 1 : 0);

	print_formated_string(value, str_value, str_value_len, operator_len, flags, width, &apply_octal_operator);
}

void print_binary_value(unsigned char flags, unsigned int width)
{
	unsigned long value = va_arg(g_ap, unsigned long);
	const char *str_value = ft_itoabase(value, 2, false);
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = ((flags & HASHTAG) && value != 0 ? 2 : 0);

	print_formated_string(value, str_value, str_value_len, operator_len, flags, width, &apply_binary_operator);
}

void print_char_value(unsigned char flags, unsigned int width)
{
	int value = va_arg(g_ap, int);
	const char str_value[] = {value, 0};
	const int str_value_len = 1;
	const int operator_len = 0;

	flags &= MINUS; // All flags are ignored but '-'

	print_formated_string(value, str_value, str_value_len, operator_len, flags, width, NULL);
}

void print_string_value(unsigned char flags, unsigned int width)
{
	char* value = va_arg(g_ap, char*);
	const char *str_value = value;
	const int str_value_len = ft_strlen(str_value);
	const int operator_len = 0;

	flags &= MINUS; // All flags are ignored but '-'

	print_formated_string(0, str_value, str_value_len, operator_len, flags, width, NULL);
}