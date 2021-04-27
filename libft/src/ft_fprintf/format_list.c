/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   format_list.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:33:01 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/27 09:24:18 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"

extern const char *color_codes[13][2];


void update_format_list(t_printf **format_list, enum e_type type, char *str)
{
	t_printf *node = *format_list;

	if (str == NULL)
		return;
	if (node == NULL) {
		(*format_list) = (t_printf*)ft_memalloc(sizeof(t_printf));
		(*format_list)->type = type;
		(*format_list)->str = str;
		return;
	}

	while(node->next != NULL)
		node = node->next;

	node->next = (t_printf*)ft_memalloc(sizeof(t_printf));
	node->next->type = type;
	node->next->str = str;
}

void free_format_list(t_printf **format_list)
{
	t_printf *node = *format_list;
	t_printf *tmp;

	while (node) {
		tmp = node;
		node = node->next;
		ft_memdel((void**)&tmp->str);
		ft_memdel((void**)&tmp);
	}
	*format_list = NULL;
}

void print_format_list(int fd, t_printf *format_list)
{
	char *convert;

	for (; format_list; format_list = format_list->next) {
		switch (format_list->type) {
			case STRING:
				ft_putstr_fd(fd, format_list->str);
				break;
			case COLOR:
				ft_putstr_fd(fd, get_color_code(format_list->str));
				break;
			case CONVERT:
				convert = convert_string(format_list->str);
				ft_putstr_fd(fd, convert);
				ft_memdel((void**)&convert);
				break;
			default:
				break;
		}
	}
}

t_printf *create_format_list(const char *str)
{
	t_printf *format_list = NULL;
	int start = 0, len = 0;

	for (len = 0; str[start + len]; len++) {
		enum e_type type;
		char *token;

		if ((token = is_color_converter(str + start + len)))
			type = COLOR;
		else if ((token = is_format_converter(str + start + len))) 
			type = CONVERT;
		if (token) {
			update_format_list(&format_list, STRING, ft_strsub(str, start, len));
			update_format_list(&format_list, type, token);
			start += len + ft_strlen(token);
			len = -1;
		}
	}
	
	update_format_list(&format_list, STRING, ft_strsub(str, start, len));
	return format_list;
}