/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 11:12:03 by anonymous     #+#    #+#                 */
/*   Updated: 2021/04/07 15:54:11 by anonymous     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

#define FLAGS "-0+ #"
#define FORMAT "diobxXscp%"

#define LEFT_JUSTIFY 0x01
#define ZERO_FILL 0x02
#define EXPLICIT_SIGN 0x04
#define POSITIVE_BLANK 0x08
#define EXPLICIT_PREFIX 0x10

#define IS_SEPARATOR(c) (c == '{' || c == '%')

const char *color_codes[13][2] = {{"{RED}", "\x1b[31m"}, {"{GREEN}", "\x1b[32m"}, {"{YELLOW}", "\x1b[33m"},
	{"{BLUE}", "\x1b[34m"}, {"{MAGENTA}", "\x1b[35m"}, {"{CYAN}", "\x1b[36m"}, {"{BOLD}", "\x1b[1m"}, 
	{"{DIM}", "\x1b[2m"}, {"{UNDERLINED}", "\x1b[4m"}, {"{BLINK}", "\x1b[5m"}, {"{REVERSE}", "\x1b[7m"},
	{"{HIDDEN}", "\x1b[8m"}, {"{EOC}", "\x1b[0m"}
};

typedef struct s_printf {
	enum e_type {
		COLOR, CONVERT, STRING
	}				type;
	char			*str;
	struct s_printf	*next;
} t_printf;

void ft_fprintf(int fd, const char *format, ...);
