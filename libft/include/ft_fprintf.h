/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 11:12:03 by anonymous     #+#    #+#                 */
/*   Updated: 2021/05/06 11:27:38 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft.h"
#include <stdbool.h> // bool
#include <stdint.h> // uintmax_t
#include <stdarg.h> // va_list

#define FLAGS "-0+ #"
#define FORMAT "diobxXscp%"

#define MINUS 0x01
#define ZERO 0x02
#define PLUS 0x04
#define BLANK 0x08
#define HASHTAG 0x10
#define CAPITAL 0x20

typedef enum {
	COLOR, CONVERT, STRING
} t_type;

typedef struct {
	t_type		type;
	const char	*str;
	size_t		size;
}				t_format;

typedef struct {
	const char	*name;
	const char	*code;
}				t_color;

extern int g_fd;
extern va_list g_ap;

// apply_operator.c
void apply_dec_operators(char **aptr, uintmax_t value, unsigned int flags);
void apply_hex_operator(char **aptr, uintmax_t value, unsigned int flags);
void apply_octal_operator(char **aptr, uintmax_t value, unsigned int flags);
void apply_binary_operator(char **aptr, uintmax_t value, unsigned int flags);

// convert_string.c
void format_and_print_string(const char *str, size_t size);

// format_list.c
void free_format(void *data);
void print_format(void *data);
t_list *create_format_list(const char *str);

// ft_fprintf.c
void ft_fprintf(int fd, const char *format, ...);

// get_value.c
void print_int_value(unsigned char flags, unsigned int width);
void print_hex_value(unsigned char flags, unsigned int width);
void print_octal_value(unsigned char flags, unsigned int width);
void print_binary_value(unsigned char flags, unsigned int width);
void print_char_value(unsigned char flags, unsigned int width);
void print_string_value(unsigned char flags, unsigned int width);