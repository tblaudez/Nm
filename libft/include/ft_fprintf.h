/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: anonymous <anonymous@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 11:12:03 by anonymous     #+#    #+#                 */
/*   Updated: 2021/05/11 11:22:42 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAXBUF (sizeof(long int) * 8)

typedef struct {
	const char	*name;
	const char	*code;
}				t_color;


void ft_fprintf(int fd, const char *format, ...);