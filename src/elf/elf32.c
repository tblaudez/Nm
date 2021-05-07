/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   elf32.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tblaudez <tblaudez@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/03 08:35:31 by tblaudez      #+#    #+#                 */
/*   Updated: 2021/05/06 08:10:23 by tblaudez      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"
#include "libft.h"
#include "ft_fprintf.h"
#include <elf.h> // Elf32_Ehdr, Elf32_Shdr
#include <stddef.h> // NULL
#include <stdlib.h> // free

extern bool g_swap_endian;

static t_list *create_symbol_node(const Elf32_Sym *symbol, const char *symstrtab, \
	const Elf32_Shdr *shdr, const char *shstrtab)
{
	t_symbol *custom_symbol = ft_memalloc(sizeof(t_symbol));

	custom_symbol->name = symstrtab + SWAP32(symbol->st_name);
	custom_symbol->st_type = ELF32_ST_TYPE(SWAP8(symbol->st_info));
	custom_symbol->st_bind = ELF32_ST_BIND(SWAP8(symbol->st_info));
	custom_symbol->st_shndx = SWAP16(symbol->st_shndx);
	custom_symbol->st_value = SWAP32(symbol->st_value);

	if (!(custom_symbol->st_shndx >= SHN_LORESERVE && custom_symbol->st_shndx <= SHN_HIRESERVE)) {
		const Elf32_Shdr *sh = &shdr[custom_symbol->st_shndx];
		custom_symbol->section.name = shstrtab + SWAP32(sh->sh_name);
		custom_symbol->section.sh_type = SWAP32(sh->sh_type);
		custom_symbol->section.sh_flags = SWAP32(sh->sh_flags);
	}

	return ft_lstnew((void*)custom_symbol);
}

t_list *elf32(const char *mapping)
{
	const Elf32_Ehdr *ehdr = (Elf32_Ehdr*)mapping;
	const Elf32_Shdr *shdr = (void*)ehdr + SWAP32(ehdr->e_shoff);
	const char *shstrtab = (void*)ehdr + SWAP32(shdr[SWAP16(ehdr->e_shstrndx)].sh_offset);

	Elf32_Sym *symtab = NULL;
	t_list *symbol_list = NULL;
	size_t symtab_size = 0;
	const char *symstrtab = NULL;
	
	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (SWAP32(shdr[i].sh_type) == SHT_SYMTAB) {
			symtab = (void*)ehdr + SWAP32(shdr[i].sh_offset);
			symstrtab = (void*)ehdr + SWAP32(shdr[SWAP32(shdr[i].sh_link)].sh_offset);
			symtab_size = SWAP32(shdr[i].sh_size) / SWAP32(shdr[i].sh_entsize);
			break;
		}
	}

	for (size_t i = 0; i < symtab_size; i++) {
		if (SWAP32(symtab[i].st_name) != 0)
			ft_lstadd_back(&symbol_list, create_symbol_node(symtab + i, symstrtab, shdr, shstrtab));
	}

	return symbol_list;
}