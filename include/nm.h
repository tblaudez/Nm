/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.h                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:42 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/05 12:05:31 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libft.h" // t_list
#include <stdint.h> // uint16_t, uint32_t
#include <stddef.h> // size_t

#define SWAP8(x) (g_swap_endian ? x : x)
#define SWAP16(x) (g_swap_endian ? __builtin_bswap16(x) : x)
#define SWAP32(x) (g_swap_endian ? __builtin_bswap32(x) : x)
#define SWAP64(x) (g_swap_endian ? __builtin_bswap64(x) : x)

void elf_common(const char *mapping, const char *filename);
t_list *elf64(const char *mapping);
t_list *elf32(const char *mapping);

typedef struct {
	const char		*name;
	unsigned char	st_type;
	unsigned char	st_bind;
	uint16_t		st_shndx;
	struct {
		const char *name;
		uint32_t sh_type;
		size_t sh_flags;
	}				section;
	size_t			st_value;
	size_t			width;
}					t_symbol;