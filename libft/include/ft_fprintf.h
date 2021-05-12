/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_fprintf.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/11 11:42:36 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/11 11:42:38 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAXBUF (sizeof(long int) * 8)

typedef struct {
	const char	*name;
	const char	*code;
}				t_color;


void ft_fprintf(int fd, const char *format, ...);