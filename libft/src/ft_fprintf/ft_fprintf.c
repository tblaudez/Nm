/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 10:42:23 by anonymous     #+#    #+#                 */
/*   Updated: 2021/04/13 14:58:38 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h> // va_list
#include "ft_fprintf.h"

va_list g_ap;
const char *color_codes[13][2] = {
	{"{RED}", "\x1b[31m"}, {"{GREEN}", "\x1b[32m"}, {"{YELLOW}", "\x1b[33m"},
	{"{BLUE}", "\x1b[34m"}, {"{MAGENTA}", "\x1b[35m"}, {"{CYAN}", "\x1b[36m"},
	{"{BOLD}", "\x1b[1m"}, {"{DIM}", "\x1b[2m"}, {"{UNDERLINED}", "\x1b[4m"},
	{"{BLINK}", "\x1b[5m"}, {"{REVERSE}", "\x1b[7m"}, {"{HIDDEN}", "\x1b[8m"},
	{"{EOC}", "\x1b[0m"}
	};

void ft_fprintf(int fd, const char *format, ...)
{
	t_printf *format_list;

	va_start(g_ap, format);

	format_list = create_format_list(format);
	print_format_list(fd, format_list);
	free_format_list(&format_list);
	
	va_end(g_ap);
}