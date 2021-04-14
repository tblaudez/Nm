/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 11:12:03 by anonymous     #+#    #+#                 */
/*   Updated: 2021/04/14 10:26:11 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h> // bool
#include <stdint.h> // uintmax_t

#define FLAGS "-0+ #"
#define FORMAT "diobxXscp%"

#define MINUS 0x01
#define ZERO 0x02
#define PLUS 0x04
#define BLANK 0x08
#define HASHTAG 0x10
#define CAPITAL 0x20


typedef struct s_printf {
	enum e_type {
		COLOR, CONVERT, STRING
	}				type;
	char			*str;
	struct s_printf	*next;
} t_printf;


// apply_operator.c
void apply_dec_operators(char **aptr, uintmax_t value, unsigned int flags);
void apply_hex_operator(char **aptr, uintmax_t value, unsigned int flags);
void apply_octal_operator(char **aptr, uintmax_t value, unsigned int flags);
void apply_binary_operator(char **aptr, uintmax_t value, unsigned int flags);

// convert_string.c
char *convert_string(const char *str);

// format_list.c
void update_format_list(t_printf **format_list, enum e_type type, char *str);
void free_format_list(t_printf **format_list);
void print_format_list(int fd, t_printf *format_list);
t_printf *create_format_list(const char *str);

// ft_fprintf.c
void ft_fprintf(int fd, const char *format, ...);

// get_value.c
char *get_int_value(unsigned char flags, unsigned int width);
char *get_hex_value(unsigned char flags, unsigned int width);
char *get_octal_value(unsigned char flags, unsigned int width);
char *get_binary_value(unsigned char flags, unsigned int width);
char *get_char_value(unsigned char flags, unsigned int width);
char *get_string_value(unsigned char flags, unsigned int width);

// utils.c
const char *get_color_code(const char *color_string);
char *is_color_converter(const char *str);
char *is_format_converter(const char *str);