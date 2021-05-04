/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.h                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:42 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/04 12:20:56 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft.h" // t_list
#include <stdint.h> // uint16_t, uint32_t
#include <stddef.h> // size_t

void elf(const char *mapping, const char *filename);
t_list *elf64(const char *mapping);
t_list *elf32(const char *mapping);

typedef struct {
	const char *name;
	unsigned char st_type;
	unsigned char st_bind;
	uint16_t st_shndx;
	size_t st_value;
	struct {
		const char *sh_name;
		uint32_t sh_type;
		size_t sh_flags;
	} section;
} t_symbol;