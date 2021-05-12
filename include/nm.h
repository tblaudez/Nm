/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nm.h                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:05:42 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/12 11:09:05 by tblaudez      ########   odam.nl         */
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
}					t_symbol;



/* ELF */
// elf_common.c
void elf_common(const char *mapping, const char *filename);

// elf64.c
t_list *elf64(const char *mapping);

// elf32.c
t_list *elf32(const char *mapping);

// get_type.c
char get_type_by_flag(const t_symbol *symbol);
char get_type_by_section(const t_symbol *symbol);
char get_type_by_index(const t_symbol *symbol);
char get_type(const t_symbol *symbol);


/* ARCHIVE */
// archive.c
void archive(const char *mapping, size_t file_size);