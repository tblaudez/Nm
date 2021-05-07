/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   format_list.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:33:01 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/07 09:37:28 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"
#include <stddef.h> // size_t
#include <unistd.h> // write

extern int g_fd;
static t_color colors[] = {
	{"{RED}", "\e[31m"}, {"{GREEN}", "\e[32m"}, {"{YELLOW}", "\e[33m"}, \
	{"{BLUE}", "\e[34m"}, {"{MAGENTA}", "\e[35m"}, {"{CYAN}", "\e[36m"}, \
	{"{BOLD}", "\e[1m"}, {"{DIM}", "\e[2m"}, {"{UNDERLINED}", "\e[4m"}, \
	{"{BLINK}", "\e[5m"}, {"{REVERSE}", "\e[7m"}, {"{HIDDEN}", "\e[8m"}, \
	{"{EOC}", "\e[0m"}
};


void print_format(void *data)
{
	t_format *format = (t_format*)data;

	switch (format->type) {
		case STRING:
			write(g_fd, format->str, format->size); break;
		case COLOR:
			ft_putstr_fd(g_fd, format->str); break;
		case CONVERT:
			format_and_print_string(format->str, format->size); break;
		default:
			break;
	}
}

static t_list *create_node(t_type type, const char *str, size_t size)
{
	if (str == NULL || size == 0)
		return NULL;

	t_format *node = (t_format*)ft_memalloc(sizeof(t_format));
	node->type = type;
	node->str = str;
	node->size = size;

	return ft_lstnew((void*)node);
}

static int find_color_index(const char *str)
{
	for (size_t i = 0; i < sizeof(colors) / sizeof(*colors); i++) {
		if (!ft_strncmp(colors[i].name, str, ft_strlen(colors[i].name))) {
			return i;
		}
	}
	return -1;
}

static int find_format_size(const char *str)
{
	int i;
	char *ptr = NULL;
	
	// Test for '%'
	if (*str != '%')
		return -1;
	// Test for flags
	for (i = 1; ft_strchr(FLAGS, str[i]); i++);
	// Test for width
	if (str[i] == '*')
		i++;
	else if (ft_strtol(str + i, &ptr, 10))
		i += (int)(ptr - (str + i));
	// Test for format
	if (!ft_strchr(FORMAT, str[i]))
		return -1;
	
	return i + 1;
}

t_list *create_format_list(const char *str)
{
	t_list *format_list = NULL;
	size_t len = 0;

	while (*(str + len)) {
		int color_index;
		if ((color_index = find_color_index(str + len)) != -1) {
			ft_lstadd_back(&format_list, create_node(STRING, str, len));
			ft_lstadd_back(&format_list, create_node(COLOR, colors[color_index].code, ft_strlen(colors[color_index].code)));
			str += len + ft_strlen(colors[color_index].name);
			len = 0;
			continue;
		}

		int format_size;
		if ((format_size = find_format_size(str + len)) != -1) {
			ft_lstadd_back(&format_list, create_node(STRING, str, len));
			ft_lstadd_back(&format_list, create_node(CONVERT, str + len, format_size));
			str += len + format_size;
			len = 0;
			continue;
		}

		len++;
	}

	ft_lstadd_back(&format_list, create_node(STRING, str, len));
	return format_list;
}