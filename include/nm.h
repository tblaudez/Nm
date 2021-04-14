/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.h                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:42 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/04/14 13:31:53 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h> // uint64_t

typedef struct s_symbol {
	uint64_t value;
	char *name;
	unsigned char type;
} t_symbol;