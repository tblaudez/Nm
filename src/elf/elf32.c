/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf32.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/03 08:35:31 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/05 12:08:27 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // Elf32_Ehdr, Elf32_Shdr
#include <stddef.h> // NULL
#include <stdlib.h> // free

static const Elf32_Ehdr *ehdr = NULL;
static const Elf32_Shdr *shdr = NULL;
static const char *symtab_strtab = NULL;
static const char *shdr_strtab = NULL;
extern bool g_swap_endian;

static t_list *create_symbol_node(const Elf32_Sym *symbol)
{
	t_symbol *custom_symbol = ft_memalloc(sizeof(t_symbol));

	custom_symbol->name = symtab_strtab + SWAP32(symbol->st_name);
	custom_symbol->st_type = ELF32_ST_TYPE(SWAP8(symbol->st_info));
	custom_symbol->st_bind = ELF32_ST_BIND(SWAP8(symbol->st_info));
	custom_symbol->st_shndx = SWAP16(symbol->st_shndx);
	custom_symbol->st_value = SWAP32(symbol->st_value);
	custom_symbol->width = 8;

	if (!(custom_symbol->st_shndx >= SHN_LORESERVE && custom_symbol->st_shndx <= SHN_HIRESERVE)) {
		const Elf32_Shdr *sh = &shdr[custom_symbol->st_shndx];
		
		custom_symbol->section.name = shdr_strtab + SWAP32(sh->sh_name);
		custom_symbol->section.sh_type = SWAP32(sh->sh_type);
		custom_symbol->section.sh_flags = SWAP32(sh->sh_flags);
	}

	return ft_lstnew((void*)custom_symbol);
}

t_list *elf32(const char *mapping)
{
	Elf32_Sym *symtab = NULL;
	t_list *symbol_list = NULL;
	size_t symtab_size = 0;

	ehdr = (Elf32_Ehdr*)mapping;
	shdr = (void*)ehdr + SWAP32(ehdr->e_shoff);
	shdr_strtab = (void*)ehdr + SWAP32(shdr[SWAP16(ehdr->e_shstrndx)].sh_offset);
	
	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (SWAP32(shdr[i].sh_type) == SHT_SYMTAB) {
			symtab = (void*)ehdr + SWAP32(shdr[i].sh_offset);
			symtab_strtab = (void*)ehdr + SWAP32(shdr[SWAP32(shdr[i].sh_link)].sh_offset);
			symtab_size = SWAP32(shdr[i].sh_size) / SWAP32(shdr[i].sh_entsize);
			break;
		}
	}

	for (size_t i = 0; i < symtab_size; i++) {
		if (SWAP32(symtab[i].st_name) != 0)
			ft_lstadd_back(&symbol_list, create_symbol_node(symtab + i));
	}

	return symbol_list;
}