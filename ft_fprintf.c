/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:42:23 by anonymous     #+#    #+#                 */
/*   Updated: 2021/04/07 16:18:16 by anonymous     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h> // va_arg
#include <stddef.h> // size_t
#include <stdlib.h> // calloc
#include "ft_fprintf.h"
#include "libft.h"

void update_format_list(t_printf **format_list, enum e_type type, char *str)
{
	t_printf *node = *format_list;

	if (node == NULL) {
		(*format_list) = (t_printf*)calloc(1, sizeof(t_printf));
		(*format_list)->type = type;
		(*format_list)->str = str;
		return;
	}

	while(node->next != NULL)
		node = node->next;

	node->next = (t_printf*)calloc(1, sizeof(t_printf));
	node->next->type = type;
	node->next->str = str;
}

char *is_color_converter(const char *str)
{
	for (size_t i = 0; i < sizeof(color_codes) / sizeof(*color_codes); i++) {
		if (ft_strncmp(str, color_codes[i][0], ft_strlen(color_codes[i][0])))
			return ft_strdup(color_codes[i][0]);
	}
	return NULL;
}

char *is_format_converter(const char *str)
{
	return NULL;
}

void split_format(const char *str)
{
	t_printf *format_list = NULL;
	size_t start = 0, len = 0;
	char *color, *format;

	while (str[start + len]) {
		if ((color = is_color_converter(str + start + len)) || (format = is_format_converter(str + start + len))) {
			update_format_list(&format_list, STRING, ft_strsub(str, start, len));
			if (color) {
				update_format_list(&format_list, COLOR, color);
				start += ft_strlen(color);
			}
			else if (format) {
				update_format_list(&format_list, CONVERT, format);
				start += ft_strlen(format);
			}
			start += len;
			len = 0;
			continue;
		}
		len++;
	}
	update_format_list(&format_list, STRING, ft_strsub(str, start, len));
}

void ft_fprintf(int fd, const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	split_format(format);
	va_end(ap);
}

int main(void) {
	ft_fprintf(1, "Hello %s {RED}World{EOC}\nI am %-0+ #5025d years old\n", "there", 21);
}







/**
 * %d %i : integers
 * %o : octal
 * %b binary
 * %x %X : Hexadecimal
 * %c : char
 * %s : string
 * %p : pointer
 * %% : '%'
 **/

/**
 * - : justify left instead of right (- supersets 0)
 * 0 : justify with 0s instead of blank
 * + : always show sign of number (+ supersets blank)
 * blank : positive values begin with blank
 * # : add prefix for %x %X %b %o 
 **/

/**
 * [%] [flags: -0+ #] [minimum field width: %5d %*d]
 **/

/**
 * {RED}Lorem Ipsum{EOC}
 **/