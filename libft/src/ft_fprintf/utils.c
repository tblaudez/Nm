/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/13 14:34:32 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/14 13:40:08 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include "libft.h"

extern const char *color_codes[13][2];


const char *get_color_code(const char *color_string)
{
	for (size_t i = 0; i < sizeof(color_codes) / sizeof(*color_codes); i++) {
		if (!ft_strcmp(color_string, color_codes[i][0]))
			return color_codes[i][1];
	}
	return color_string;
}

char *is_color_converter(const char *str)
{
	for (size_t i = 0; i < sizeof(color_codes) / sizeof(*color_codes); i++) {
		if (!ft_strncmp(str, color_codes[i][0], ft_strlen(color_codes[i][0])))
			return ft_strdup(color_codes[i][0]);
	}
	return NULL;
}

char *is_format_converter(const char *str)
{
	int i;
	char *ptr = NULL;
	
	// Test for '%'
	if (*str != '%')
		return NULL;
	// Test for flags
	for(i = 1; ft_strchr(FLAGS, str[i]); i++);
	// Test for width
	if (str[i] == '*')
		i++;
	else if (ft_strtol(str + i, &ptr, 10))
		i += (int)(ptr - (str + i));
	// Test for format
	if (!ft_strchr(FORMAT, str[i]))
		return NULL;
	
	return ft_strsub(str, 0, i + 1);
}