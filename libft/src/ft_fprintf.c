/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/11 11:42:19 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/11 11:42:25 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <stdarg.h> // va_list, va_arg, va_end

static t_color colors[] = {
	{"{RED}", "\e[31m"}, {"{GREEN}", "\e[32m"}, {"{YELLOW}", "\e[33m"}, \
	{"{BLUE}", "\e[34m"}, {"{MAGENTA}", "\e[35m"}, {"{CYAN}", "\e[36m"}, \
	{"{BOLD}", "\e[1m"}, {"{DIM}", "\e[2m"}, {"{UNDERLINED}", "\e[4m"}, \
	{"{BLINK}", "\e[5m"}, {"{REVERSE}", "\e[7m"}, {"{HIDDEN}", "\e[8m"}, \
	{"{EOC}", "\e[0m"}
};


void format_string(const char *str, va_list *ap, int fd)
{
	int length, prec, plus_sign, base, capitals;
	bool ladjust, altfmt;
	char padc, sign_char, *endptr;
	unsigned long u;
	register char c;
	long n;

	while ((c = *str) != '\0') {
		if (c == '{') {
			bool found = false;
			for (size_t i = 0; i < sizeof(colors) / sizeof(*colors); i++) {
				if (ft_strncmp(str, colors[i].name, ft_strlen(colors[i].name)) == 0) {
					found = true;
					ft_putstr_fd(fd, colors[i].code);
					str += ft_strlen(colors[i].name);
					break;
				}
			}
			if (found) 
				continue;
		}
		if (c != '%') {
			ft_putchar_fd(fd, c);
			str++;
			continue;
		}
		str++;

		length = 0;
	    prec = -1;
	    ladjust = false;
	    padc = ' ';
	    plus_sign = 0;
	    sign_char = 0;
	    altfmt = false;
	
		while (true) {
			c = *str;
			if (c == '#')
				altfmt = true;
			else if (c == '-')
				ladjust = true;
			else if (c == '+')
				plus_sign = '+';
			else if (c == ' ') {
				if (plus_sign == 0)
					plus_sign = ' ';
			}
			else
				break;
			str++;
		}

		if (c == '0') {
			padc = '0';
			c = *++str;
		}
		
		if (ft_isdigit(c)) {
			length = (int)ft_strtol(str, &endptr, 10);
			str = endptr;
			c = *str;
		}
		else if (c == '*') {
			length = va_arg(*ap, int);
			c = *++str;
			if (length < 0) {
				ladjust = !ladjust;
				length = -length;
			}
		}

		if (c == '.') {
			c = *++str;
			if (ft_isdigit(c)) {
				prec = (int)ft_strtol(str, &endptr, 10);
				str =  endptr;
				c = *str;
			}
			else if (c == '*') {
				prec = va_arg(*ap, int);
				c = *++str;
			}
		}

		if (c == 'l')
			c = *++str;

		capitals = 0;
		
		switch (c) {
			case 'c':
				c = va_arg(*ap, int);
				ft_putchar_fd(fd, c);
				break;
			
			case 's':
			{
				register char *p;
				register char *p2;

				if (prec == -1)
					prec = INT32_MAX;
				
				p = va_arg(*ap,  char*);
				if (p == NULL)
					p = "";
				
				if (length > 0 && !ladjust) {
					n = 0;
					p2 = p;
					for (; *p !=  '\0' && n < prec; p++)
						n++;
					p = p2;
					while (n < length) {
						ft_putchar_fd(fd, ' ');
						n++;
					}
				}

				n = 0;
				while (*p != '\0') {
					if (++n > prec || (length > 0 && n > length))
						break;
					ft_putchar_fd(fd, *p++);
				}
				if (n < length && ladjust) {
					while (n < length) {
						ft_putchar_fd(fd, ' ');
						n++;
					}
				}
				break;
			}

			case 'o':
			case 'O':
				base = 8;
				goto print_unsigned;

			case 'd':
				base = 10;
				goto print_signed;
			
			case 'u':
				base = 10;
				goto print_unsigned;
			
			case 'p':
				altfmt = true;
			case 'x':
				base = 16;
				goto print_unsigned;
			case 'X':
				base = 16;
				capitals  = 16;
				goto print_unsigned;
			
			case 'b':
				base = 2;
				goto print_unsigned;

			print_signed:
				n = (long)va_arg(*ap, int);
				if (n >= 0) {
					u = n;
					sign_char = plus_sign;
				} else {
					u = -n;
					sign_char = '-';
				}
				goto print_num;
			
			print_unsigned:
				u = va_arg(*ap, unsigned long);
				goto print_num;
			
			print_num:
			{
				char buf[MAXBUF];
				register char *p = &buf[MAXBUF-1];
				static char digits[] = "0123456789abcdef0123456789ABCDEF";
				char *prefix = 0;

				if (u != 0 && altfmt) {
					if (base == 8)
						prefix = "0";
					else if (base == 16)
						prefix = (capitals ? "0X" : "0x");	
					else if (base == 2)
						prefix = "0b";			
				}
				do {
					*p-- = digits[(u % base) + capitals];
					u /= base;
				} while (u != 0);
				length -= (&buf[MAXBUF-1] - p);
				if (sign_char)
					length--;
				if (prefix)
					length -= ft_strlen(prefix);
				
				if (padc == ' ' && !ladjust) {
					while (--length >= 0)
						ft_putchar_fd(fd, ' ');
				}
				
				if (sign_char)
					ft_putchar_fd(fd, sign_char);
				if (prefix)
					ft_putstr_fd(fd, prefix);
				if (padc  == '0')
					while (--length >= 0)
						ft_putchar_fd(fd, '0');
				while (++p != &buf[MAXBUF])
					ft_putchar_fd(fd, *p);
				if (ladjust)
					while (--length >= 0)
						ft_putchar_fd(fd, ' ');
				
				break;
			}

			case '\0':
				str--;
				break;

			default:
				ft_putchar_fd(fd, c);
		}
		str++;
	}
}

void ft_fprintf(int fd, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	format_string(format, &ap, fd);
	va_end(ap);
}